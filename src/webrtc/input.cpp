/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"
#include"gadget/gadget.h"
#include"gadget/gadget_ctx.h"
#include"input/input.h"
#include"input/input_ctx.h"

void WebRTCInstance::OnInputMessage(rtc::message_variant data){
	auto bin=std::get<rtc::binary>(data);
	size_t len=bin.size(),add;
	auto ptr=reinterpret_cast<uint8_t*>(bin.data());
	while(len>=sizeof(InputEvent)){
		auto event=(InputEvent*)ptr;
		try{
			kvm->usb.input->Report(event);
			add=sizeof(InputEvent);
		}catch(...){
			add=1;
		}
		ptr+=add,len-=add;
	}
}

void WebRTCInstance::OnMouseModeEvent(Json::Value&event){
	try{
		auto ms=event["mode"].asString();
		auto mode=StringToMouseMode(ms);
		if(mode==MOUSE_UNKNOWN)return;
		log_info("try change mouse mode to {}",ms);
		GadgetSetMouseMode(kvm,mode);
		log_info("changed mouse mode to {}",ms);
		Json::Value event;
		event["type"]="mouse_mode_changed";
		event["mode"]=ms;
		ep->SendEventAll(event);
	}catch(std::exception&exc){
		log_warn("failed to set mouse mode: {}",exc.what());
	}
}
