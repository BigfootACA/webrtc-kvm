/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"screen.h"
#include"lib/log.h"
#include"string/strings.h"
#include<QMetaEnum>

void WebRTCScreen::onStateChange(rtc::PeerConnection::State state){
	log_info("state changed to {}",RTCPeerConnectionStateToString(state));
	if(state==rtc::PeerConnection::State::Connected)setStatus(Connected);
	if(state==rtc::PeerConnection::State::Closed)setStatus(Disconnected);
}

void WebRTCScreen::onGatheringStateChange(rtc::PeerConnection::GatheringState state){
	log_info("gathering state changed to {}",RTCPeerConnectionGatheringStateToString(state));
}

void WebRTCScreen::onLocalDescription(rtc::Description desc){
	log_info("local description {}",desc.typeString());
	if(desc.type()==rtc::Description::Type::Answer){
		Json::Value req;
		req["id"]=webrtc_id.ToString();
		req["sdp"]=std::string(desc);
		HttpCall("/api/webrtc/set_sdp",req,[](auto,auto){
			log_info("webrtc set sdp successful");
		});
	}
}

void WebRTCScreen::onTrack(std::shared_ptr<rtc::Track>track){
	auto mid=track->mid();
	log_info("on track mid {}",mid);
	if(mid=="video")onVideoTrack(track);
}

void WebRTCScreen::onDataChannel(std::shared_ptr<rtc::DataChannel>ch){
	auto label=ch->label();
	log_info("on data channel label {}",label);
	if(label=="event")onEventChannel(ch);
	if(label=="input")onInputChannel(ch);
}

void WebRTCScreen::InitWebRTC(){
	setStatus(Initializing);
	pc=std::make_shared<rtc::PeerConnection>();
	pc->onStateChange([this](auto state){QMetaObject::invokeMethod(this,"onStateChange",state);});
	pc->onDataChannel([this](auto channel){QMetaObject::invokeMethod(this,"onDataChannel",channel);});
	pc->onGatheringStateChange([this](auto state){QMetaObject::invokeMethod(this,"onGatheringStateChange",state);});
	pc->onLocalDescription([this](auto desc){QMetaObject::invokeMethod(this,"onLocalDescription",desc);});
	pc->onTrack([this](auto track){QMetaObject::invokeMethod(this,"onTrack",track);});
}
