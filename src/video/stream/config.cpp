/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/exception.h"

void Stream::LoadConfig(YAML::Node&cfg){
	if(!cfg)throw InvalidArgument("bad config");
	if(!uuid.IsNull()||!id.empty()||!name.empty())
		throw InvalidArgument("stream config already loaded");
	uuid.Generate();
	id=std::format("stream-{}-{}",GetDriverName(),uuid.ToString());
	name=std::format("Stream@{}({})",GetDriverName(),uuid.ToString());
	if(auto v=cfg["id"])id=v.as<std::string>();
	if(auto v=cfg["name"])name=v.as<std::string>();
	OnLoadDeviceConfig(cfg);
}
