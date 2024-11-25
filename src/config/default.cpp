/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc_kvm.h"
#include"video/stream.h"

void ConfigPopulateDefault(webrtc_kvm*ctx){
	ctx->streams=new StreamList;
	ctx->video.auto_stop=true;
	ctx->http.port=2345;
	ctx->usb.name="webrtc-kvm";
	ctx->usb.manufacturer="ClassFun";
	ctx->usb.product="WebRTC-KVM";
	ctx->usb.id_product=0x0104;
	ctx->usb.id_vendor=0x1d6b;
	ctx->usb.enable=true;
	ctx->video.fps=30;
	ctx->http.auth.no_auth=true;
}
