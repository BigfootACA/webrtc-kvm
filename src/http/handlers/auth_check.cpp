/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

class HttpAuthCheckHandler:public HttpRestfulHandler{
	public:
		inline HttpAuthCheckHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/auth/check";}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpAuthCheckHandler http_auth_check_handler;

int HttpAuthCheckHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	try{
		UUID u;
		if(auto v=ctx->request["token"])u=v.asString();
		if(u.IsNull())throw InvalidArgument("null uuid");
		ctx->token=ctx->auth->GetAuthToken(u);
	}catch(std::exception&exc){
		throw HttpError(MHD_HTTP_BAD_REQUEST,"bad token");
	}
	if(!ctx->token||!ctx->token->Valid())
		throw HttpError(MHD_HTTP_UNAUTHORIZED,"token invalid");
	return MHD_HTTP_OK;
}
