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

void WebRTCInstance::OnStateChange(rtc::PeerConnection::State p_state){
	std::unique_lock<std::mutex>lk(lock);
	const char*str;
	switch(p_state){
		case rtc::PeerConnection::State::New:str="new";break;
		case rtc::PeerConnection::State::Connecting:str="connecting";break;
		case rtc::PeerConnection::State::Connected:str="connected";break;
		case rtc::PeerConnection::State::Disconnected:str="disconnected";break;
		case rtc::PeerConnection::State::Failed:str="failed";break;
		case rtc::PeerConnection::State::Closed:str="closed";break;
		default:str="unknown";break;
	}
	log_info("webrtc {} state change to {}",uuid.ToString(),str);
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
