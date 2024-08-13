/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"webrtc/webrtc.h"

class HttpStreamWebRTCSetSDPHandler:public HttpRestfulHandler{
	public:
		inline HttpStreamWebRTCSetSDPHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/webrtc/set_sdp";}
		[[nodiscard]] bool NeedAuth()const final{return true;}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpStreamWebRTCSetSDPHandler http_stream_webrtc_set_sdp_handler;

int HttpStreamWebRTCSetSDPHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	auto id=ctx->request["id"].asString();
	auto sdp=ctx->request["sdp"].asString();
	auto webrtc=dynamic_cast<WebRTCEndpoint*>(ctx->ctx->streams->FindByID("webrtc"));
	if(unlikely(!webrtc))throw HttpError(MHD_HTTP_SERVICE_UNAVAILABLE,"no webrtc stream");
	auto instance=webrtc->GetInstance(UUID(id));
	if(unlikely(!instance))throw HttpError(MHD_HTTP_NOT_FOUND,"no such webrtc instance");
	try{
		instance->SetSDP(sdp);
	}catch(std::exception&exc){
		log_warn("failed to set sdp: {}",exc.what());
		throw HttpError(MHD_HTTP_NOT_ACCEPTABLE,"request set sdp failed");
	}
	return MHD_HTTP_OK;
}
