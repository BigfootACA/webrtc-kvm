/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"

void V4L2Device::InitializeDevice(){
	if(status!=STREAM_UNINITIALIZE)
		throw InvalidArgument("device already initialized");
	if(device_fd<0){
		if(path.empty())FindMatchDevice();
		OpenDevice();
	}
	std::string driver,card;
	v4l2_capabilities_probe(device_fd,device_cap,driver,card);
	type=DetectType(device_cap);
	v4l2_buffers_capabilities_probe(device_fd,type,memory,buffer_cap);
	log_info("v4l2 {} using {} with driver {} card {}",GetID(),GetDriverName(),driver,card);
	SetupFormat();
	SetupBuffer();
	InitializeBuffers();
	for(const auto&kv:controls)try{
		log_dbg("v4l2 {} set control 0x{:X} to {}",GetID(),kv.first,kv.second);
		v4l2_control ctrl={.id=kv.first,.value=kv.second};
		v4l2_control_set(device_fd,ctrl);
	}catch(std::exception&exc){
		log_warn("v4l2 {} set control 0x{:X} failed: {}",GetID(),kv.first,exc.what());
	}
}
