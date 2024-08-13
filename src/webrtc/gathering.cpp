/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"
#include"lib/log.h"

void WebRTCInstance::OnGatheringStateChange(rtc::PeerConnection::GatheringState p_state){
	std::unique_lock<std::mutex>lk(lock);
	const char*str;
	switch(p_state){
		case rtc::PeerConnection::GatheringState::New:str="new";break;
		case rtc::PeerConnection::GatheringState::InProgress:str="inprogress";break;
		case rtc::PeerConnection::GatheringState::Complete:str="complete";break;
		default:str="unknown";break;
	}
	log_info("webrtc {} gathering state change to {}",uuid.ToString(),str);
	this->gathering_state=p_state;
	if(p_state==rtc::PeerConnection::GatheringState::Complete){
		auto lsdp=peer->localDescription();
		if(lsdp->type()==rtc::Description::Type::Offer){
			local_sdp=lsdp.value();
		}
	}
}
