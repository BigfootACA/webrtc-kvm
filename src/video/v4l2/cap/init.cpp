/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_cap.h"

void V4L2Capture::OnInitialize(){
	if(outputs.empty())throw InvalidArgument("no link");
	InitializeDevice();
}

void V4L2Capture::OnDeinitialize(){
	V4L2Device::OnDeinitialize();
}

void V4L2Capture::SetupDevice(){
	if(edid)SetupEdid();
	if(subdev)SetupSubdev();
}

v4l2_buf_type V4L2Capture::DetectType(uint32_t cap){
	if(have_bit(cap,V4L2_CAP_VIDEO_M2M_MPLANE)||have_bit(cap,V4L2_CAP_VIDEO_CAPTURE_MPLANE)){
		log_info("Capture using multiple plane API");
		return V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	}else if(have_bit(cap,V4L2_CAP_VIDEO_M2M)||have_bit(cap,V4L2_CAP_VIDEO_CAPTURE)){
		log_info("Capture using single plane API");
		return V4L2_BUF_TYPE_VIDEO_CAPTURE;
	}else throw InvalidArgument("target device is not a V4L2 Capture");
}

void V4L2Capture::OnLoadDeviceConfig(YAML::Node&cfg){
	LoadGenericConfig(cfg);
	LoadEdidConfig(cfg);
	if(auto v=cfg["subdev"])subdev=v.as<bool>();
	if(auto v=cfg["mbus_code"])mbus_code=v.as<uint32_t>();
}
