/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"
#include"lib/log.h"
#include"string/strings.h"

void WebRTCInstance::OnGatheringStateChange(rtc::PeerConnection::GatheringState p_state){
	std::unique_lock<std::mutex>lk(lock);
	log_info(
		"webrtc {} gathering state change to {}",
		uuid.ToString(),RTCPeerConnectionGatheringStateToString(p_state)
	);
	this->gathering_state=p_state;
	if(p_state==rtc::PeerConnection::GatheringState::Complete){
		auto lsdp=peer->localDescription();
		if(lsdp->type()==rtc::Description::Type::Offer){
			local_sdp=lsdp.value();
		}
	}
}
