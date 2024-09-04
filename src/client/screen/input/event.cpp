/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"input.h"

std::shared_ptr<InputEventProcess>InputEventProcess::FindInput(QObject*begin){
	QObject*x=begin;
	WebRTCScreen*screen=nullptr;
	do{
		screen=dynamic_cast<WebRTCScreen*>(x);
		x=x->parent();
	}while(x&&!screen);
	if(!screen)return nullptr;
	return screen->input_ch;
}

void VideoBox::keyPressEvent(QKeyEvent*event){
	auto ch=InputEventProcess::FindInput(this);
	if(ch)ch->onKeyEvent(event,InputEventProcess::PRESS);
}

void VideoBox::keyReleaseEvent(QKeyEvent*event){
	auto ch=InputEventProcess::FindInput(this);
	if(ch)ch->onKeyEvent(event,InputEventProcess::RELEASE);
}

void VideoBox::mousePressEvent(QMouseEvent*event){
	auto ch=InputEventProcess::FindInput(this);
	if(ch)ch->onMouseEvent(event,InputEventProcess::PRESS);
}

void VideoBox::mouseReleaseEvent(QMouseEvent*event){
	auto ch=InputEventProcess::FindInput(this);
	if(ch)ch->onMouseEvent(event,InputEventProcess::RELEASE);
}

void VideoBox::mouseMoveEvent(QMouseEvent*event){
	auto ch=InputEventProcess::FindInput(this);
	if(ch)ch->onMouseEvent(event,InputEventProcess::MOVE);
}

#if QT_CONFIG(wheelevent)
void VideoBox::wheelEvent(QWheelEvent*event){
	auto ch=InputEventProcess::FindInput(this);
	if(ch)ch->onWheelEvent(event);
}
#endif
