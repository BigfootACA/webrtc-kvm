/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"webrtc/webrtc.h"

class HttpStreamWebRTCGetSDPHandler:public HttpRestfulHandler{
	public:
		inline HttpStreamWebRTCGetSDPHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/webrtc/get_sdp";}
		[[nodiscard]] bool NeedAuth()const final{return true;}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpStreamWebRTCGetSDPHandler http_stream_webrtc_get_sdp_handler;

int HttpStreamWebRTCGetSDPHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	auto id=ctx->request["id"].asString();
	auto webrtc=dynamic_cast<WebRTCEndpoint*>(ctx->ctx->streams->FindByID("webrtc"));
	if(unlikely(!webrtc))throw HttpError(MHD_HTTP_SERVICE_UNAVAILABLE,"no webrtc stream");
	auto instance=webrtc->GetInstance(UUID(id));
	if(unlikely(!instance))throw HttpError(MHD_HTTP_NOT_FOUND,"no such webrtc instance");
	auto local_sdp=instance->GetSDP();
	if(local_sdp.empty())throw HttpError(MHD_HTTP_NOT_ACCEPTABLE,"get sdp failed");
	ctx->response["sdp"]=local_sdp;
	return MHD_HTTP_OK;
}
