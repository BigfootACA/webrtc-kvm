/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"
#include"input/input.h"
#include"input/input_ctx.h"
#include"lib/log.h"
#include"abstract/async-runner.h"
#include"string/strings.h"

void WebRTCInstance::OnStateChange(rtc::PeerConnection::State p_state){
	std::unique_lock<std::mutex>lk(lock);
	log_info(
		"webrtc {} state change to {}",
		uuid.ToString(),RTCPeerConnectionStateToString(p_state)
	);
	this->state=p_state;
	if(p_state==rtc::PeerConnection::State::Connected){
		AsyncRunner::DefaultRunLater([this](auto){
			EnableStream();
		});
	}
	if(p_state==rtc::PeerConnection::State::Disconnected){
		AsyncRunner::DefaultRunLater([this](auto){
			DisableStream();
			kvm->usb.input->ResetReport();
		});
	}
	if(p_state==rtc::PeerConnection::State::Closed){
		AsyncRunner::DefaultRunLater([this](auto){
			this->peer->close();
			ep->RemoveStream(uuid);
		});
	}
}
