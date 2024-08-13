/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"

void WebRTCInstance::EnableStream(){
	std::unique_lock<std::mutex>lk(lock);
	ep->EnableStream(uuid);
}

void WebRTCInstance::DisableStream(){
	std::unique_lock<std::mutex>lk(lock);
	ep->DisableStream(uuid);
}

void WebRTCEndpoint::EnableStream(const UUID&uuid){
	std::unique_lock<std::mutex>lk(lock);
	auto stream=GetInstance(uuid);
	if(!stream)return;
	auto idx=std::find(streaming.begin(),streaming.end(),stream);
	if(idx!=streaming.end())return;
	if(streaming.empty()){
		log_info("new client request auto start stream");
		StartStream(true);
	}
	streaming.push_back(stream);
}

void WebRTCEndpoint::DisableStream(const UUID&uuid){
	std::unique_lock<std::mutex>lk(lock);
	auto stream=GetInstance(uuid);
	if(!stream)return;
	auto idx=std::find(streaming.begin(),streaming.end(),stream);
	if(idx==streaming.end())return;
	streaming.erase(idx);
	if(streaming.empty()&&kvm->video.auto_stop){
		log_info("no active client, auto stop stream");
		StopStream(true);
	}
}

void WebRTCEndpoint::RemoveStream(const UUID&uuid){
	DisableStream(uuid);
	auto idx=instances.find(uuid);
	if(idx==instances.end())return;
	auto ins=idx->second;
	instances.erase(idx);
	delete ins;
}
