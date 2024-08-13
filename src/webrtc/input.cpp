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

void WebRTCInstance::OnInputMessage(rtc::message_variant data){
	auto bin=std::get<rtc::binary>(data);
	size_t len=bin.size(),add;
	auto ptr=reinterpret_cast<uint8_t*>(bin.data());
	while(len>=sizeof(InputEvent)){
		auto event=(InputEvent*)ptr;
		try{
			kvm->usb.input->Report(event);
			add=sizeof(InputEvent);
		}catch(...){
			add=1;
		}
		ptr+=add,len-=add;
	}
}
