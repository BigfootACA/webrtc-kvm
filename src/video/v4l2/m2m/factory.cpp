/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"

cdecl_attr_used V4L2MemoryToMemoryFactory v4l2_memory_to_memory_factory;

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

v4l2_buf_type V4L2MemoryToMemory::DetectType(uint32_t){
	throw InvalidArgument("V4L2MemoryToMemory DetectType disabled");
}
