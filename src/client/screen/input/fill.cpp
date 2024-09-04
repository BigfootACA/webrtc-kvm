/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"input.h"

bool InputEventProcess::onKeyEvent(QKeyEvent*event,EventType type){
	InputEvent ev{};
	switch(type){
		case PRESS:ev.type=EVENT_KEY_DOWN;break;
		case RELEASE:ev.type=EVENT_KEY_UP;break;
		default:return false;
	}
	ev.key.code=map->Translate(event);
	if(ev.key.code==KEY_CODE_RESERVED)return false;
	SendInputEvent(ev);
	return true;
}

bool InputEventProcess::onMouseEvent(QMouseEvent*event,EventType type){
	InputEvent ev{};
	switch(type){
		case PRESS:ev.type=EVENT_ABSOLUTE_DOWN;break;
		case RELEASE:ev.type=EVENT_ABSOLUTE_UP;break;
		case MOVE:ev.type=EVENT_ABSOLUTE_MOVE;break;
		default:return false;
	}
	switch(event->button()){
		case Qt::NoButton:ev.absolute.btn=(InputMouseButton)0;break;
		case Qt::LeftButton:ev.absolute.btn=BTN_MOUSE_LEFT;break;
		case Qt::RightButton:ev.absolute.btn=BTN_MOUSE_RIGHT;break;
		case Qt::MiddleButton:ev.absolute.btn=BTN_MOUSE_WHEEL;break;
		default:return false;
	}
	auto point=event->pos();
	auto size=screen->video->size();
	auto px=point.x()*UINT16_MAX/size.width();
	auto py=point.y()*UINT16_MAX/size.height();
	ev.absolute.pos_x=px,ev.absolute.pos_y=py;
	SendInputEvent(ev);
	return true;
}

bool InputEventProcess::onWheelEvent(QWheelEvent*event){
	InputEvent ev{};
	ev.type=EVENT_MOUSE_WHEEL;
	auto p=event->angleDelta();
	ev.wheel.delta_x=p.x();
	ev.wheel.delta_y=p.y();
	SendInputEvent(ev);
	return true;
}

bool InputEventProcess::onTabletEvent(QTabletEvent*event,EventType type){
	/* TODO: unimplemented */
	return false;
}
