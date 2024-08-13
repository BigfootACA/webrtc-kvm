/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"webrtc/webrtc.h"
#include"algo/uuid.h"

class HttpStreamWebRTCNewHandler:public HttpRestfulHandler{
	public:
		inline HttpStreamWebRTCNewHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/webrtc/new";}
		[[nodiscard]] bool NeedAuth()const final{return true;}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpStreamWebRTCNewHandler http_stream_webrtc_new_handler;

int HttpStreamWebRTCNewHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	std::pair<UUID,WebRTCInstance*>p;
	auto webrtc=dynamic_cast<WebRTCEndpoint*>(ctx->ctx->streams->FindByID("webrtc"));
	if(unlikely(!webrtc))throw HttpError(MHD_HTTP_SERVICE_UNAVAILABLE,"no webrtc stream");
	try{
		p.first.Generate();
		p.second=nullptr;
		webrtc->NewInstance(p);
		if(unlikely(!p.second))throw RuntimeError("create webrtc instance failed");
		ctx->response["id"]=p.first.ToString();
	}catch(std::exception&exc){
		log_warn("failed to create webrtc instance: {}",exc.what());
		throw HttpError(MHD_HTTP_INTERNAL_SERVER_ERROR,"create webrtc instance failed");
	}
	return MHD_HTTP_OK;
}
