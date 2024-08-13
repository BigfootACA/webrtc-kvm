/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<cstdlib>
#include"webrtc_kvm.h"
#include"lib/full_io.h"
#include"lib/exception.h"
#include"hid.h"
#include"input.h"
#include"input_ctx.h"

#define WRITE_REPORT(type1,type2)do{\
	if(hid_##type1##_fd<0)break;\
	full_write(hid_##type1##_fd,&report.type1,sizeof(report.type1));\
}while(0)
#define FILL_WRITE_REPORT(type1,type2)do{\
	if(hid_##type1##_fd<0)break;\
	InputFillHID##type2(&report.type1,ev); \
	WRITE_REPORT(type1,type2);\
}while(0)

void InputContext::Report(const InputEvent*ev){
	if(unlikely(!ev||memcmp(ev->magic,EVENT_MAGIC,sizeof(ev->magic))!=0))
		throw InvalidArgument("bad input event");
	switch(ev->type){
		case EVENT_KEY_DOWN:
		case EVENT_KEY_UP:
			FILL_WRITE_REPORT(keyboard,Keyboard);
		break;
		case EVENT_MOUSE_WHEEL:
			FILL_WRITE_REPORT(absolute,Absolute);
			FILL_WRITE_REPORT(relative,Relative);
		break;
		case EVENT_ABSOLUTE_DOWN:
		case EVENT_ABSOLUTE_UP:
		case EVENT_ABSOLUTE_MOVE:
			FILL_WRITE_REPORT(absolute,Absolute);
		break;
		case EVENT_RELATIVE_DOWN:
		case EVENT_RELATIVE_UP:
		case EVENT_RELATIVE_MOVE:
			FILL_WRITE_REPORT(relative,Relative);
		break;
		case EVENT_TOUCH_START:
		case EVENT_TOUCH_MOVE:
		case EVENT_TOUCH_END:
		case EVENT_TOUCH_CANCEL:
			FILL_WRITE_REPORT(touchscreen,Touchscreen);
		break;
		default:;
	}
}

void InputContext::ResetReport(){
	memset(&report,0,sizeof(report));
	WRITE_REPORT(keyboard,Keyboard);
	WRITE_REPORT(absolute,Absolute);
	WRITE_REPORT(relative,Relative);
	WRITE_REPORT(touchscreen,Touchscreen);
}

void InputContext::Close(){
	memset(&report,0,sizeof(report));
	if(hid_keyboard_fd>=0)close(hid_keyboard_fd);
	if(hid_absolute_fd>=0)close(hid_absolute_fd);
	if(hid_relative_fd>=0)close(hid_relative_fd);
	if(hid_touchscreen_fd>=0)close(hid_touchscreen_fd);
	hid_keyboard_fd=-1;
	hid_absolute_fd=-1;
	hid_relative_fd=-1;
	hid_touchscreen_fd=-1;
}

InputContext::InputContext(webrtc_kvm*ctx):ctx(ctx){

}

void InitializeInput(webrtc_kvm*ctx){
	ctx->usb.input=new InputContext(ctx);
}
