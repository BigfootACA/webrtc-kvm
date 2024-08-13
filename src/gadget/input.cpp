/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"lib/exception.h"
#include"abstract/resource_release.h"
#include"input/hid.h"
#include"input/input_ctx.h"
#include"gadget_ctx.h"

void GadgetCloseInput(webrtc_kvm*ctx){
	auto input=ctx->usb.input;
	if(!input)return;
	log_dbg("disable gadget hid input");
	input->Close();
}

void GadgetOpenInput(webrtc_kvm*ctx){
	auto input=ctx->usb.input;
	auto gadget=ctx->usb.gadget;
	if(!input||!gadget->keyboard||!gadget->mouse)return;
	if(gadget->gadget->GetUDC().empty())return;
	log_dbg("enable gadget hid input");
	FDRelease kbd(gadget->keyboard->OpenDevice());
	FDRelease mse(gadget->mouse->OpenDevice());
	switch(gadget->last_mode){
		case MOUSE_ABSOLUTE:
			log_dbg("use hid absolute mouse");
			input->hid_absolute_fd=mse.Take();
		break;
		case MOUSE_RELATIVE:
			log_dbg("use hid relative mouse");
			input->hid_relative_fd=mse.Take();
		break;
		case MOUSE_TOUCHSCREEN:
			log_dbg("use hid touchscreen");
			input->hid_touchscreen_fd=mse.Take();
		break;
		default:throw RuntimeError("unsupported mouse mode");
	}
	input->hid_keyboard_fd=kbd.Take();
}

static void GadgetInitKeyboard(gadget_ctx*gadget){
	gadget->keyboard=std::make_shared<GadgetHID>(gadget->gadget,"keyboard");
	gadget->keyboard->SetProtocol(1);
	gadget->keyboard->SetSubClass(1);
	gadget->keyboard->SetReportLength(sizeof(HIDKeyboardReport));
	gadget->keyboard->WriteReportDescriptor(&HIDDescKeyboard);
	gadget->config->LinkFunction(gadget->keyboard);
}

static void GadgetInitMouse(gadget_ctx*gadget,mouse_mode mode){
	size_t len;
	const HIDReportDesc*desc;
	switch(mode){
		case MOUSE_ABSOLUTE:len=sizeof(HIDAbsoluteReport),desc=&HIDDescAbsolute;break;
		case MOUSE_RELATIVE:len=sizeof(HIDRelativeReport),desc=&HIDDescRelative;break;
		default:throw RuntimeError("unsupported mouse mode");
	}
	gadget->mouse=std::make_shared<GadgetHID>(gadget->gadget,"mouse");
	gadget->mouse->SetProtocol(2);
	gadget->mouse->SetSubClass(1);
	gadget->mouse->SetReportLength(len);
	gadget->mouse->WriteReportDescriptor(desc);
	gadget->config->LinkFunction(gadget->mouse);
}

void GadgetSetMouseMode(webrtc_kvm*ctx,mouse_mode mode){
	auto gadget=ctx->usb.gadget;
	if(mode==gadget->last_mode)return;
	ClaimUDC lock(gadget->gadget);
	GadgetCloseInput(ctx);
	for(const auto&func:gadget->gadget->ListFunctions())
		if(func->GetFunction()=="hid")
			func->Remove();
	GadgetInitKeyboard(gadget);
	GadgetInitMouse(gadget,mode);
	gadget->last_mode=mode;
	GadgetOpenInput(ctx);
}
