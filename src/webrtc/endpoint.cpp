/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"

void WebRTCEndpoint::ReportSize(){
	auto src=GetInputStream();
	if(!src)return;
	auto width=src->GetWidth();
	auto height=src->GetHeight();
	auto fps=src->GetFrameRate();
	if(width==0&&height==0&&fps==0)return;
	Json::Value event;
	event["type"]="resize";
	event["width"]=width;
	event["height"]=height;
	event["fps"]=fps;
	SendEventAll(event);
}

Stream*WebRTCEndpoint::ParentStream(){
	if(!input||!input->source)throw InvalidArgument("no input link");
	return input->source;
}

void WebRTCEndpoint::OnInitialize(){

}

void WebRTCEndpoint::OnDeinitialize(){

}

void WebRTCEndpoint::OnLoadDeviceConfig(YAML::Node&cfg){

}
