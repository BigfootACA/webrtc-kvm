/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_out.h"

void V4L2Output::OnInitialize(){
	if(!input)throw InvalidArgument("no link");
	if(width==0)width=input->source->GetWidth();
	if(height==0)height=input->source->GetHeight();
	if(fps==0)fps=input->source->GetFrameRate();
	InitializeDevice();
}

v4l2_buf_type V4L2Output::DetectType(uint32_t cap){
	if(have_bit(device_cap,V4L2_CAP_VIDEO_M2M_MPLANE)||have_bit(device_cap,V4L2_CAP_VIDEO_OUTPUT_MPLANE)){
		log_info("Output using multiple plane API");
		return V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	}else if(have_bit(device_cap,V4L2_CAP_VIDEO_M2M)||have_bit(device_cap,V4L2_CAP_VIDEO_OUTPUT)){
		log_info("Output using single plane API");
		return V4L2_BUF_TYPE_VIDEO_OUTPUT;
	}else throw InvalidArgument("target device is not a V4L2 Output");
}

void V4L2Output::OnDeinitialize(){
	V4L2Device::OnDeinitialize();
}
