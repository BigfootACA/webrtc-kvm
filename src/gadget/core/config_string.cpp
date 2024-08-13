/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"internal.h"
#include"gadget_config.h"
#include"gadget_config_string.h"
#include"lib/exception.h"

GadgetConfigString::GadgetConfigString(
	std::weak_ptr<const GadgetConfig>config,
	const std::string&name
):config(std::move(config)),name(name),id(std::stoi(name,nullptr,16)){
	Create();
}

GadgetConfigString::GadgetConfigString(
	std::weak_ptr<const GadgetConfig>config,
	uint16_t id
):config(std::move(config)),name(ToHexString(id,4)),id(id){
	Create();
}

GadgetConfigString::~GadgetConfigString(){

}

std::string GadgetConfigString::GetPath()const{
	auto x=config.lock();
	if(!x)throw RuntimeError("invalid config");
	return std::format("{}/strings/{}",x->GetPath(),GetName());
}

IMPL_ATTR_STRING(GadgetConfigString,Configuration,configuration)
