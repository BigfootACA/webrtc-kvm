/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"
#include"string/strings.h"
#include"config/configs.h"

void V4L2Device::LoadGenericConfig(YAML::Node&obj){
	if(auto v=obj["width"])width=v.as<uint32_t>();
	if(auto v=obj["height"])height=v.as<uint32_t>();
	if(auto v=obj["fps"])fps=v.as<uint32_t>();
	if(auto v=obj["buffers"])buffer_cnt=v.as<uint32_t>();
	if(auto v=obj["device"])path=v.as<std::string>();
	load_string_conv(obj,fourcc,fourcc,Fourcc);
	load_string_conv(obj,memory,memory,V4L2Memory);
}
