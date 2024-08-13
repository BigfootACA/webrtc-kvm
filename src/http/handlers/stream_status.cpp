/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

class HttpStreamStatusHandler:public HttpRestfulHandler{
	public:
		inline HttpStreamStatusHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/stream/status";}
		[[nodiscard]] bool NeedAuth()const final{return true;}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpStreamStatusHandler http_stream_status_handler;

int HttpStreamStatusHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	//pthread_mutex_lock(&ctx->ctx->lock);
	//ctx->response["streaming"]=ctx->ctx->video.stream_refcnt;
	//ctx->response["width"]=ctx->ctx->video.width;
	//ctx->response["height"]=ctx->ctx->video.height;
	//ctx->response["fps"]=ctx->ctx->video.fps;
	//ctx->response["frames"]=ctx->ctx->video.frames;
	//ctx->response["frames_rate"]=ctx->ctx->video.rate_frames;
	//ctx->response["outbound"]=ctx->ctx->video.outbound;
	//ctx->response["outbound_rate"]=ctx->ctx->video.rate_outbound;
	//ctx->response["fourcc_pic"]=std::string((const char*)&ctx->ctx->video.fourcc_pic,4);
	//ctx->response["fourcc_coded"]=std::string((const char*)&ctx->ctx->video.fourcc_coded,4);
	//pthread_mutex_unlock(&ctx->ctx->lock);
	return MHD_HTTP_OK;
}
