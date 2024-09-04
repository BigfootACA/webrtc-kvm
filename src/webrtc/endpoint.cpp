/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"

bool WebRTCEndpoint::GenerateReportSize(Json::Value&event){
	if(!input)return false;
	auto src=GetInputStream();
	auto width=src->GetWidth();
	auto height=src->GetHeight();
	auto fps=src->GetFrameRate();
	if(width==0&&height==0&&fps==0)return false;
	event["type"]="resize";
	event["width"]=width;
	event["height"]=height;
	event["fps"]=fps;
	return true;
}

void WebRTCEndpoint::ReportSize(){
	Json::Value event;
	if(GenerateReportSize(event))
		SendEventAll(event);
}

void WebRTCEndpoint::OnInitialize(){

}

void WebRTCEndpoint::OnDeinitialize(){

}

void WebRTCEndpoint::OnLoadDeviceConfig(YAML::Node&cfg){

}
