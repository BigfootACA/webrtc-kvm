/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

class HttpAuthInfoHandler:public HttpRestfulHandler{
	public:
		inline HttpAuthInfoHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/auth/info";}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpAuthInfoHandler http_auth_info_handler;

int HttpAuthInfoHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	bool hide_username,no_login=false;
	std::string username;
	auto&auth=ctx->ctx->http.auth;
	auto&pam=auth.pam;
	auto&pwd=auth.pwd;
	if(pam.enabled)hide_username=!pam.show_user,username=pam.default_user;
	else if(pwd.enabled)hide_username=!pwd.show_user,username=pwd.default_user;
	else if(auth.no_auth)no_login=true,hide_username=true,username="";
	else throw HttpError(MHD_HTTP_INTERNAL_SERVER_ERROR,"unexcepted login setting found");
	ctx->response["hide_username"]=hide_username;
	ctx->response["username"]=username;
	ctx->response["no_login"]=no_login;
	return MHD_HTTP_OK;
}
