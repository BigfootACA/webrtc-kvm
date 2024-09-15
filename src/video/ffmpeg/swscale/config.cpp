/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"swscale.h"
#include"config/configs.h"

void SwscalePipe::OnLoadDeviceConfig(YAML::Node&cfg){
	if(auto v=cfg["width"])width=v.as<uint32_t>();
	if(auto v=cfg["height"])height=v.as<uint32_t>();
	load_string_conv(cfg,fourcc,CurrentFourcc(),Fourcc);
}
