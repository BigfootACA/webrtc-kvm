/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

HttpAuthorization::HttpAuthorization(webrtc_kvm*ctx):ctx(ctx){}

std::shared_ptr<HttpAuthStatus>HttpAuthorization::Login(const AuthRequest&req){
	auto&auth=ctx->http.auth;
	log_info("login request with username {}",req.username);
	if(auth.no_auth)throw HttpError(MHD_HTTP_NOT_ACCEPTABLE,"auth disabled");
	else if(auth.pam.enabled)AuthPAM(req);
	else if(auth.pwd.enabled)AuthPassword(req);
	else throw HttpError(MHD_HTTP_INTERNAL_SERVER_ERROR,"unexcepted login setting found");
	log_info("login request with username {} successed",req.username);
	auto token=UUID::Random();
	while(GetAuthToken(token))token=UUID::Random();
	if(token.IsNull())throw RuntimeError("generate uuid failed");
	tokens[token]=std::make_shared<HttpAuthStatus>(token,req.username,time(nullptr));
	return tokens[token];
}

HttpAuthStatus::HttpAuthStatus(
	const UUID&token,
	const std::string&user,
	time_t time
):token(token),user(user),login_time(time){}

bool HttpAuthStatus::Valid()const{
	auto now=time(nullptr);
	if(token.IsNull())return false;
	if(user.empty())return false;
	if(now-login_time>HTTP_AUTH_VALID_TIME)return false;
	return true;
}

std::shared_ptr<HttpAuthStatus>HttpAuthorization::GetAuthToken(const UUID&token){
	if(token.IsNull())return nullptr;
	auto i=tokens.find(token);
	if(i==tokens.end())return nullptr;
	return i->second;
}

std::shared_ptr<HttpAuthStatus>HttpAuthorization::GetAuthTokenFromHeader(const HttpHeaders&hdrs){
	if(!hdrs.Have(MHD_HTTP_HEADER_AUTHORIZATION))return nullptr;
	auto auth=hdrs.GetOne(MHD_HTTP_HEADER_AUTHORIZATION);
	if(auth.empty()||!auth.starts_with("Bearer "))return nullptr;
	try{
		UUID u(auth.substr(7));
		if(u.IsNull())throw InvalidArgument("null uuid");
		return GetAuthToken(u);
	}catch(...){
		return nullptr;
	}
}

void HttpAuthorization::Logout(const UUID&token){
	if(token.IsNull())return;
	auto i=tokens.find(token);
	if(i==tokens.end())return;
	log_info("logout {}",i->second->GetUser());
	tokens.erase(i);
}

void HttpAuthorization::Logout(std::shared_ptr<HttpAuthStatus>token){
	if(token)Logout(token->GetToken());
}
