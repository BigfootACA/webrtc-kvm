/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"

void WebRTCInstance::Init(){
	rtc::Configuration cfg;
	peer=std::make_shared<rtc::PeerConnection>(cfg);
	if(unlikely(!peer))throw RuntimeError("create WebRTC peer connection failed");
	peer->onDataChannel([this](auto dc){OnDataChannel(dc);});
	peer->onStateChange([this](auto s){OnStateChange(s);});
	peer->onGatheringStateChange([this](auto s){OnGatheringStateChange(s);});
	VideoTrackInit(video,96,42,"video","stream");
	if(unlikely(!(input=peer->createDataChannel("input"))))
		throw RuntimeError("create WebRTC input channel failed");
	if(unlikely(!(event=peer->createDataChannel("event"))))
		throw RuntimeError("create WebRTC event channel failed");
	input->onMessage([this](auto val){OnInputMessage(val);});
	event->onMessage([this](auto val){OnEventMessage(val);});
	event->onOpen([this](){OnEventOpen();});
	peer->setLocalDescription();
}

void WebRTCInstance::NewInstance(WebRTCEndpoint*ep,std::pair<UUID,WebRTCInstance*>&ins){
	if(ins.first.IsNull())ins.first.Generate();
	if(unlikely(ep->instances.find(ins.first)!=ep->instances.end()))
		throw RuntimeError("webrtc instance already exists");
	auto instance=new WebRTCInstance;
	if(unlikely(!instance))throw RuntimeError("create WebRTC instance failed");
	log_info("create new webrtc instance {} in {}",ins.first.ToString(),ep->GetID());
	instance->uuid=ins.first;
	instance->ep=ep;
	instance->kvm=ep->kvm;
	instance->Init();
	ins.second=instance;
	ep->instances.insert(ins);
}

void WebRTCEndpoint::NewInstance(std::pair<UUID,WebRTCInstance*>&ins){
	std::unique_lock<std::mutex>lk(lock);
	return WebRTCInstance::NewInstance(this,ins);
}
