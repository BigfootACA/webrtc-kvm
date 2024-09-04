/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"hid.h"
#include"input.h"

void InputFillHIDKeyboard(HIDKeyboardReport*r,const InputEvent*ev){
	bool pressed;
	HIDKeyMod mod;
	HIDKeyCode code;
	if(unlikely(!r||!ev))return;
	switch(ev->type){
		case EVENT_KEY_DOWN:pressed=true;break;
		case EVENT_KEY_UP:pressed=false;break;
		default:return;
	}
	if((mod=InputKeyToHIDMod(ev->key.code))!=0){
		set_bit(r->mod,mod,pressed);
		return;
	}
	if((code=InputKeyToHIDCode(ev->key.code))!=0){
		for(uint32_t i=0;i<ARRAY_SIZE(r->key);i++){
			if(r->key[i]!=(pressed?HID_KEY_UNKNOWN:code))continue;
			r->key[i]=pressed?code:HID_KEY_UNKNOWN;
			break;
		}
	}
}

static void parse_button(InputMouseButton mb,InputEventType type,HIDMouseBtn*dest){
	HIDMouseBtn btn;
	switch(mb){
		case BTN_MOUSE_LEFT:btn=HID_BTN_MOUSE_LEFT;break;
		case BTN_MOUSE_WHEEL:btn=HID_BTN_MOUSE_MIDDLE;break;
		case BTN_MOUSE_RIGHT:btn=HID_BTN_MOUSE_RIGHT;break;
		default:return;
	}
	switch(type){
		case EVENT_ABSOLUTE_UP:case EVENT_RELATIVE_UP:set_bit(*dest,btn,false);break;
		case EVENT_ABSOLUTE_DOWN:case EVENT_RELATIVE_DOWN:set_bit(*dest,btn,true);break;
		default:break;
	}
}

void InputFillHIDRelative(HIDRelativeReport*r,const InputEvent*ev){
	if(unlikely(!r||!ev))return;
	if(ev->type==EVENT_MOUSE_WHEEL){
		if(ev->wheel.delta_y>0)r->wheel=-1;
		if(ev->wheel.delta_y<0)r->wheel=1;
	}else{
		r->wheel=0;
		parse_button(ev->relative.btn,ev->type,&r->btn);
		r->delta_x=(int8_t)(((int32_t)ev->relative.delta_x+32768)*254/65535-127);
		r->delta_y=(int8_t)(((int32_t)ev->relative.delta_y+32768)*254/65535-127);
	}
}

void InputFillHIDAbsolute(HIDAbsoluteReport*r,const InputEvent*ev){
	if(unlikely(!r||!ev))return;
	if(ev->type==EVENT_MOUSE_WHEEL){
		if(ev->wheel.delta_y>0)r->wheel=-1;
		if(ev->wheel.delta_y<0)r->wheel=1;
	}else{
		r->wheel=0;
		parse_button(ev->absolute.btn,ev->type,&r->btn);
		r->position_x=ev->absolute.pos_x/2;
		r->position_y=ev->absolute.pos_y/2;
	}
}

void InputFillHIDTouchscreen(HIDTouchscreenReport*r,const InputEvent*ev){
	if(unlikely(!r||!ev))return;
	if(ev->touch.finger>=10)return;
	r->contact_id=ev->touch.finger;
	switch(ev->type){
		case EVENT_TOUCH_START:
			r->contact_count++;
			r->tip_switch=true;
			r->in_range=true;
		break;
		case EVENT_TOUCH_MOVE:break;
		case EVENT_TOUCH_END:
		case EVENT_TOUCH_CANCEL:
			r->contact_count--;
			r->tip_switch=false;
			r->in_range=false;
		break;
		default:return;
	}
	r->position_x=ev->touch.pos_x/2;
	r->position_y=ev->touch.pos_y/2;
}
