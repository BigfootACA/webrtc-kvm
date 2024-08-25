/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"posix.h"

void PosixStream::OnLoadDeviceConfig(YAML::Node&cfg){
	if(auto v=cfg["url"])url.Parse(v.as<std::string>());
}

void PosixInputStream::OnLoadDeviceConfig(YAML::Node&cfg){
	PosixStream::OnLoadDeviceConfig(cfg);
}

void PosixOutputStream::OnLoadDeviceConfig(YAML::Node&cfg){
	PosixStream::OnLoadDeviceConfig(cfg);
	if(auto v=cfg["width"])width=v.as<uint32_t>();
	if(auto v=cfg["height"])height=v.as<uint32_t>();
	if(auto v=cfg["fps"])fps=v.as<uint32_t>();
}
