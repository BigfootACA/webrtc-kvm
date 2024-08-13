/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"
#include"config/configs.h"

void V4L2MemoryToMemory::OnLoadDeviceConfig(YAML::Node&cfg){
	out->OnLoadDeviceConfig(cfg);
	cap->OnLoadDeviceConfig(cfg);
	LoadGenericConfig(cfg);
	cap->fourcc=fourcc,out->fourcc=0;
	cap->memory=memory,out->memory=(v4l2_memory)0;
	cap->id=std::format("{}.capture",id);
	out->id=std::format("{}.output",id);
	cap->name=std::format("{} (capture)",name);
	out->name=std::format("{} (output)",name);
	switch(fourcc){
		case V4L2_PIX_FMT_H264:
			if(role==ROLE_ENCODER)LoadH264EncoderConfig(cfg["h264"]);
		break;
		default:;
	}
}
