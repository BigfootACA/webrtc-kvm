/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"input.h"
#include"lib/log.h"

void InputEventKeyPrint(std::string prefix,const InputEventKey*ev){
	log_dbg(
		"{}Code: {} ({})",prefix,
		InputKeyCodeToString(ev->code,"unknown"),
		(uint16_t)ev->code
	);
}

void InputEventMouseRelativePrint(std::string prefix,const InputEventMouseRelative*ev){
	log_dbg("{}Delta X: {}",prefix,ev->delta_x);
	log_dbg("{}Delta Y: {}",prefix,ev->delta_y);
	log_dbg(
		"{}Button: {} ({})",prefix,
		InputMouseButtonToString(ev->btn,"unknown"),
		(uint16_t)ev->btn
	);
}

void InputEventMouseAbsolutePrint(std::string prefix,const InputEventMouseAbsolute*ev){
	log_dbg("{}Position X: {}",prefix,ev->pos_x);
	log_dbg("{}Position Y: {}",prefix,ev->pos_y);
	log_dbg(
		"{}Button: {} ({})",prefix,
		InputMouseButtonToString(ev->btn,"unknown"),
		(uint16_t)ev->btn
	);
}

void InputEventTouchPrint(std::string prefix,const InputEventTouch*ev){
	log_dbg("{}Finger: {}",prefix,ev->finger);
	log_dbg("{}Position X: {}",prefix,ev->pos_x);
	log_dbg("{}Position Y: {}",prefix,ev->pos_y);
}

void InputEventWheelPrint(std::string prefix,const InputEventWheel*ev){
	log_dbg("{}Delta X: {}",prefix,ev->delta_x);
	log_dbg("{}Delta Y: {}",prefix,ev->delta_y);
	log_dbg("{}Delta Z: {}",prefix,ev->delta_z);
}

void InputEventPrint(std::string prefix,const InputEvent*ev){
	log_dbg(
		"{}Type: {} ({})",prefix,
		InputEventTypeToString(ev->type,"unknown"),
		(uint16_t)ev->type
	);
	prefix+="  ";
	switch(ev->type){
		case EVENT_KEY_DOWN:
		case EVENT_KEY_UP:
			InputEventKeyPrint(prefix,&ev->key);
		break;
		case EVENT_MOUSE_WHEEL:
			InputEventWheelPrint(prefix,&ev->wheel);
		break;
		case EVENT_ABSOLUTE_DOWN:
		case EVENT_ABSOLUTE_UP:
		case EVENT_ABSOLUTE_MOVE:
			InputEventMouseAbsolutePrint(prefix,&ev->absolute);
		break;
		case EVENT_RELATIVE_DOWN:
		case EVENT_RELATIVE_UP:
		case EVENT_RELATIVE_MOVE:
			InputEventMouseRelativePrint(prefix,&ev->relative);
		break;
		case EVENT_TOUCH_START:
		case EVENT_TOUCH_MOVE:
		case EVENT_TOUCH_END:
		case EVENT_TOUCH_CANCEL:
			InputEventTouchPrint(prefix,&ev->touch);
		break;
		default:;
	}
}