/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"internal.h"
#include"gadget.h"
#include"gadget_string.h"
#include"lib/exception.h"

GadgetString::GadgetString(
	std::weak_ptr<const Gadget>gadget,
	const std::string&name
):gadget(std::move(gadget)),name(name),id(std::stoi(name,nullptr,16)){
	Create();
}

GadgetString::GadgetString(
	std::weak_ptr<const Gadget>gadget,
	uint16_t id
):gadget(std::move(gadget)),name(ToHexString(id,4)),id(id){
	Create();
}

GadgetString::~GadgetString(){

}

std::string GadgetString::GetPath()const{
	auto x=gadget.lock();
	if(!x)throw RuntimeError("invalid gadget");
	return std::format("{}/strings/{}",x->GetPath(),GetName());
}

IMPL_ATTR_STRING(GadgetString,Manufacturer,manufacturer)
IMPL_ATTR_STRING(GadgetString,Product,product)
IMPL_ATTR_STRING(GadgetString,SerialNumber,serialnumber)
