/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"video/stream.h"

class HttpStreamStopHandler:public HttpRestfulHandler{
	public:
		inline HttpStreamStopHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/stream/stop";}
		[[nodiscard]] bool NeedAuth()const final{return true;}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpStreamStopHandler http_stream_stop_handler;

int HttpStreamStopHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	//int ret;
	//ret=stream_stop(ctx->ctx);
	//if(ret)throw RuntimeError("request stop stream failed: {}",ret);
	return MHD_HTTP_OK;
}
