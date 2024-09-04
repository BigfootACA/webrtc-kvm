/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"input.h"

void InputEventProcess::onOpen(){
	log_info("input channel opened");
	screen->SubmitJob([this](){
		screen->video->setMouseTracking(true);
	},Qt::AutoConnection);
}

void InputEventProcess::onClosed(){
	log_info("input channel closed");
	screen->SubmitJob([this](){
		screen->video->setMouseTracking(false);
	},Qt::AutoConnection);
}

void InputEventProcess::onError(std::string reason){
	log_warn("input channel error: {}",reason);
}

void InputEventProcess::onMessage(rtc::message_variant msg){
	auto data=std::get<rtc::binary>(msg);
	screen->current.bytes+=data.size();
	screen->current.packets+=1;
}
