/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"input.h"

InputEventProcess::InputEventProcess(
	WebRTCScreen*screen,
	std::shared_ptr<rtc::DataChannel>ch
):screen(screen),ch(ch){
	ch->onOpen([this](){onOpen();});
	ch->onClosed([this](){onClosed();});
	ch->onError([this](auto err){onError(err);});
	ch->onMessage([this](auto m){onMessage(m);});
	map=InputKeyMapFactory::CreateAuto();
}

void WebRTCScreen::onInputChannel(std::shared_ptr<rtc::DataChannel>ch){
	input_ch=std::make_shared<InputEventProcess>(this,ch);
}

void InputEventProcess::SendInputEvent(InputEvent&ev){
	try{
		union{
			InputEvent e;
			std::byte b[sizeof(InputEvent)];
		}d;
		if(!ch->isOpen())return;
		memcpy(&d.e,&ev,sizeof(d.e));
		memcpy(d.e.magic,EVENT_MAGIC,sizeof(d.e.magic));
		rtc::binary bin(std::begin(d.b),std::end(d.b));
		ch->send(bin);
	}catch(std::exception&exc){
		log_warn("send input event failed: {}",exc.what());
	}
}
