/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"
#include"lib/log.h"
#include"config/configs.h"
#include"abstract/resource_release.h"

void RockchipMediaProcessPlatform::OnLoadDeviceConfig(YAML::Node&cfg){
	if(auto v=cfg["width"])width=v.as<uint32_t>();
	if(auto v=cfg["height"])height=v.as<uint32_t>();
	if(auto v=cfg["fps"])fps=v.as<uint32_t>();
	if(auto v=cfg["config"])config_persist.FromYAML(v);
	load_string_conv(cfg,fourcc,CurrentFourcc(),Fourcc);
}
