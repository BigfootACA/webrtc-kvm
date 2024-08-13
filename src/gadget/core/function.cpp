/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"internal.h"
#include"gadget.h"
#include"gadget_config.h"
#include"gadget_function.h"
#include"lib/exception.h"
#include"lib/modules.h"

GadgetFunction::GadgetFunction(
	std::weak_ptr<const Gadget>gadget,
	const std::string&func,
	const std::string&id
):gadget(std::move(gadget)),func(func),id(id){
	if(func.empty()||!CheckValid(func,VALIDS))
		throw RuntimeError("bad func {}",func);
	if(id.empty()||!CheckValid(id,VALIDS))
		throw RuntimeError("bad id {}",id);
	LoadModule(std::format("usbfunc:{}",func));
	Create();
}

GadgetFunction::~GadgetFunction(){
}

std::string GadgetFunction::GetPath()const{
	auto x=gadget.lock();
	if(!x)throw RuntimeError("invalid gadget");
	return std::format("{}/functions/{}",x->GetPath(),GetName());
}

std::map<std::shared_ptr<GadgetConfig>,std::string>GadgetFunction::ListLinkedConfigs()const{
	std::map<std::shared_ptr<GadgetConfig>,std::string>res;
	auto g=gadget.lock();
	if(!g)throw RuntimeError("invalid gadget");
	auto cfg_name=GetName();
	for(const auto&cfg:g->ListConfigs())
		for(const auto&kv:cfg->ListLinkedFunctions())
			if(kv.second->GetName()==cfg_name)
				res[cfg]=kv.first;
	return res;
}

void GadgetFunction::Remove(){
	for(const auto&linked:ListLinkedConfigs())
		linked.first->RemoveLinkedFunction(linked.second);
	RemoveFolder(GetPath());
}

void GadgetFunction::LinkToConfig(
	const std::shared_ptr<const GadgetConfig>&cfg,
	const std::string&link_name
)const{
	cfg->LinkFunction(shared_from_this(),link_name);
}
