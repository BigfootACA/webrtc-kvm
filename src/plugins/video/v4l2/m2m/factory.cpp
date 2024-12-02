/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"

V4L2MemoryToMemory::V4L2MemoryToMemory(webrtc_kvm*ctx){
	this->ctx=ctx;
	out=new V4L2Output(ctx);
	cap=new V4L2Capture(ctx);
	out->open_device=false;
	cap->open_device=false;
}

Stream*V4L2MemoryToMemoryFactory::Create(webrtc_kvm*ctx){
	return new V4L2MemoryToMemory(ctx);
}

V4L2MemoryToMemory::~V4L2MemoryToMemory(){
	delete cap;
	delete out;
}

DECL_FACTORY(V4L2MemoryToMemoryFactory,v4l2_m2m)
