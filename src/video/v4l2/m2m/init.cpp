/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"

void V4L2MemoryToMemory::OnInitialize(){
	if(!input||!output)throw InvalidArgument("no link");
	OpenDevice();
	cap->device_fd=device_fd,out->device_fd=device_fd;
	cap->controls.clear();
	out->controls.clear();
	out->controls=controls;
	auto inp=GetInputStream();
	if(width==0)width=inp->GetWidth();
	if(height==0)height=inp->GetHeight();
	if(fps==0)fps=inp->GetFrameRate();
	if(out->width==0)out->width=width;
	if(out->height==0)out->height=height;
	if(out->fps==0)out->fps=fps;
	out->OnInitialize();
	if(cap->width==0)cap->width=out->width;
	if(cap->height==0)cap->height=out->height;
	if(cap->fps==0)cap->fps=out->fps;
	cap->OnInitialize();
	switch(role){
		case ROLE_ENCODER:OnInitializeEncoder();break;
		case ROLE_DECODER:OnInitializeDecoder();break;
		default:;
	}
}

void V4L2MemoryToMemory::OnDeinitialize(){
	out->OnDeinitialize();
	cap->OnDeinitialize();
	V4L2Device::OnDeinitialize();
}
