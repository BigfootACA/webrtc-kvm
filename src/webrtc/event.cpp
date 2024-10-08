/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"
#include"gadget/gadget_ctx.h"

void WebRTCInstance::OnEventOpen(){
	ReportSize();
	Json::Value event;
	event["type"]="mouse_mode_changed";
	event["mode"]=MouseModeToString(kvm->usb.gadget->last_mode);
	SendEvent(event);
}

void WebRTCInstance::OnEventMessage(rtc::message_variant data){
	try{
		Json::Value node;
		Json::Reader reader;
		auto payload=std::get<rtc::string>(data);
		if(!reader.parse(payload,node))throw RuntimeError(
			"bad json: {}",reader.getFormattedErrorMessages()
		);
		auto type=node["type"].asString();
		log_info("got event {} in webrtc {}",type,uuid.ToString());
		if(type=="mouse_mode_set")OnMouseModeEvent(node);
	}catch(std::exception&exc){
		log_warn(
			"failed to handle event in webrtc {}: {}",
			uuid.ToString(),exc.what()
		);
	}
}

void WebRTCInstance::ReportSize(){
	Json::Value event;
	if(ep->GenerateReportSize(event))
		SendEvent(event);
}

void WebRTCInstance::SendEvent(Json::Value&node){
	try{
		Json::FastWriter writer;
		SendEvent(writer.write(node));
	}catch(...){}
}

void WebRTCInstance::SendEvent(const std::string&str){
	try{
		event->send(str);
	}catch(...){}
}

void WebRTCEndpoint::SendEventAll(Json::Value&node){
	try{
		Json::FastWriter writer;
		auto str=writer.write(node);
		for(const auto&kv:instances)
			kv.second->SendEvent(str);
	}catch(...){}
}
