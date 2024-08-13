/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"

class WebRTCEndpointFactory:public StreamFactory{
	public:
		~WebRTCEndpointFactory()override=default;
		inline WebRTCEndpointFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()final{return "webrtc";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

cdecl_attr_used WebRTCEndpointFactory webrtc_endpoint_factory;

Stream*WebRTCEndpointFactory::Create(webrtc_kvm*ctx){
	return new WebRTCEndpoint(ctx);
}

WebRTCInstance::WebRTCInstance(){

}

WebRTCInstance::~WebRTCInstance(){
	DisableStream();
	log_info("destroy webrtc instance {}",uuid.ToString());
}

WebRTCInstance*WebRTCEndpoint::GetInstance(const UUID&uuid){
	auto i=instances.find(uuid);
	if(i==instances.end())return nullptr;
	return i->second;
}
