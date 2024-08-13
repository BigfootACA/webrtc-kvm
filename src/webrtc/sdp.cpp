/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"

void WebRTCInstance::SetSDP(const std::string&sdp){
	if(unlikely(
		gathering_state!=rtc::PeerConnection::GatheringState::Complete
	))throw InvalidArgument("connection is not ready for set sdp");
	rtc::Description desc(std::string(sdp),rtc::Description::Type::Answer);
	peer->setRemoteDescription(desc);
}

std::string WebRTCInstance::GetSDP()const{
	return local_sdp;
}
