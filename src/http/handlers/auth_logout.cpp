/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

class HttpAuthLogoutHandler:public HttpRestfulHandler{
	public:
		inline HttpAuthLogoutHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/auth/logout";}
		[[nodiscard]] bool NeedAuth()const final{return true;}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpAuthLogoutHandler http_auth_logout_handler;

int HttpAuthLogoutHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	AuthRequest req;
	ctx->auth->Logout(ctx->token);
	return MHD_HTTP_OK;
}
