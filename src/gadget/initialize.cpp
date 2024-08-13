/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"gadget_ctx.h"
#include"lib/log.h"

void InitializeGadget(webrtc_kvm*ctx){
	gadget_ctx*gadget;
	if(!ctx->usb.gadget)ctx->usb.gadget=new gadget_ctx;
	gadget=ctx->usb.gadget;
	gadget->last_mode=MOUSE_UNKNOWN;
	gadget->gadget=std::make_shared<Gadget>(ctx->usb.name);
	auto udc=gadget->gadget->GetUDC();
	if(!udc.empty()){
		ctx->usb.udc=udc;
		log_info("found previous UDC {}",udc);
		gadget->gadget->SetUDC("");
	}
	log_info(
		"USB device {:04X}:{:04X} {} {}",
		ctx->usb.id_vendor,ctx->usb.id_product,
		ctx->usb.manufacturer,ctx->usb.product
	);
	gadget->gadget->Recreate();
	gadget->gadget->SetVendorID(ctx->usb.id_vendor);
	gadget->gadget->SetProductID(ctx->usb.id_product);
	gadget->gadget->SetBcdUSB(0x0200);
	gadget->gadget->SetBcdDevice(0x0100);
	gadget->gadget->AddString(0x409,ctx->usb.manufacturer,ctx->usb.product);
	gadget->config=gadget->gadget->AddConfig("a",1);
	gadget->config->AddString(0x409,"KVM");
	GadgetSetMouseMode(ctx,MOUSE_ABSOLUTE);
	if(ctx->usb.udc.empty()){
		auto detected=Gadget::DetectUDC();
		if(!detected.empty()){
			log_info("auto selected UDC {}",detected);
			ctx->usb.udc=detected;
		}else log_warn("no any udc found");
	}
	if(!ctx->usb.udc.empty()){
		log_info("enable UDC {}",ctx->usb.udc);
		gadget->gadget->SetUDC(ctx->usb.udc);
		GadgetOpenInput(ctx);
	}
}
