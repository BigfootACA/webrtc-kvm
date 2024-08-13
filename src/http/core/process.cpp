/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

MHD_Result HttpRequestContext::CatchRun(std::function<MHD_Result(HttpRequestContext*)>run){
	int code=MHD_HTTP_SERVICE_UNAVAILABLE;
	Json::Value res;
	try{
		return run(this);
	}catch(Exceptions::HttpError&e){
		code=e.code;
		res["message"]=e.GetOriginal();
		log_dbg("request {} failed: {}",url,e.what());
	}catch(std::exception&e){
		code=MHD_HTTP_INTERNAL_SERVER_ERROR;
		res["message"]="unexpected exception occurred";
		log_err("unknown exception occurred while process {}: {}",url,e.what());
	}catch(...){
		code=MHD_HTTP_INTERNAL_SERVER_ERROR;
		res["message"]="unknown exception occurred";
		log_err("unknown exception occurred while process {}",url);
	}
	return HandleJSON(code,res);
}

MHD_Result HttpHandler::Process(HttpRequestContext*ctx){
	auto hand_method=GetMethod();
	if(ctx->method==HTTP_METHOD_OPTIONS&&!have_bit(hand_method,HTTP_METHOD_OPTIONS))
		return ctx->HandleString(MHD_HTTP_NO_CONTENT,nullptr,"");
	if(hand_method!=0&&!have_bit(hand_method,ctx->method))
		throw HttpError(MHD_HTTP_METHOD_NOT_ALLOWED,"method unsupported");
	if(NeedAuth()&&!ctx->ctx->http.auth.no_auth){
		auto status=ctx->auth->GetAuthTokenFromHeader(ctx->req_headers);
		if(!status)throw HttpError(MHD_HTTP_UNAUTHORIZED,"authorization failed");
		ctx->token=status;
	}
	return Handle(ctx);
}
