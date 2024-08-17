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

void V4L2Device::LoadMatchConfig(YAML::Node&obj){
	if(!obj.IsDefined())return;
	if(!obj.IsMap())throw InvalidArgument("bad match config");
	if(auto v=obj["card"])match.card=v.as<std::string>();
	if(auto v=obj["driver"])match.driver=v.as<std::string>();
}

void V4L2Device::LoadGenericConfig(YAML::Node&obj){
	if(auto v=obj["width"])width=v.as<uint32_t>();
	if(auto v=obj["height"])height=v.as<uint32_t>();
	if(auto v=obj["fps"])fps=v.as<uint32_t>();
	if(auto v=obj["buffers"])buffer_cnt=v.as<uint32_t>();
	if(auto v=obj["match"])LoadMatchConfig(v);
	load_string_conv(obj,fourcc,CurrentFourcc(),Fourcc);
	load_string_conv(obj,memory,memory,V4L2Memory);
}
