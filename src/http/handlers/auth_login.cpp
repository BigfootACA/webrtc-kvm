/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"algo/uuid.h"

class HttpAuthLoginHandler:public HttpRestfulHandler{
	public:
		inline HttpAuthLoginHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/auth/login";}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpAuthLoginHandler http_auth_login_handler;

int HttpAuthLoginHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	AuthRequest req;
	if(auto v=ctx->request["username"])req.username=v.asString();
	if(auto v=ctx->request["password"])req.password=v.asString();
	if(req.username.empty()||req.password.empty())
		throw HttpError(MHD_HTTP_BAD_REQUEST,"no username or password set");
	auto status=ctx->auth->Login(req);
	if(!status||!status->Valid())
		throw HttpError(MHD_HTTP_UNAUTHORIZED,"login failed");
	ctx->token=status;
	ctx->response["token"]=status->GetToken().ToString();
	return MHD_HTTP_OK;
}
