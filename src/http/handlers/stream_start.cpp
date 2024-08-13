/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"video/stream.h"

class HttpStreamStartHandler:public HttpRestfulHandler{
	public:
		inline HttpStreamStartHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/stream/start";}
		[[nodiscard]] bool NeedAuth()const final{return true;}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpStreamStartHandler http_stream_start_handler;

int HttpStreamStartHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	//int ret;
	//pthread_mutex_lock(&ctx->ctx->lock);
	//ret=stream_start(ctx->ctx);
	//pthread_mutex_unlock(&ctx->ctx->lock);
	//if(ret)throw RuntimeError("request start stream failed: {}",ret);
	return MHD_HTTP_OK;
}
