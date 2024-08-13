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
#include"gadget_config_string.h"
#include"gadget_function.h"
#include"lib/path.h"
#include"lib/exception.h"

static void check(
	GadgetConfig*self,
	const std::string&id,
	const uint8_t num
){
	if(num<=0)throw InvalidArgument("invalid num for gadget config");
	if(id.empty()||!CheckValid(id,VALIDS))
		throw RuntimeError("bad id {}",id);
}

GadgetConfig::GadgetConfig(
	std::weak_ptr<const Gadget>gadget,
	const std::string&name
):gadget(std::move(gadget)){
	auto i=name.find('.');
	if(i==std::string::npos)
		throw RuntimeError("bad name {}",name);
	this->name=name;
	this->id=name.substr(0,i);
	this->num=std::stoi(name.substr(i+1));
	check(this,id,num);
	Create();
}

GadgetConfig::GadgetConfig(
	std::weak_ptr<const Gadget>gadget,
	const std::string&id,
	const uint16_t num
):gadget(std::move(gadget)),name(std::format("{}.{}",id,num)),id(id),num(num){
	check(this,id,num);
	Create();
}

GadgetConfig::~GadgetConfig(){

}

std::vector<std::shared_ptr<GadgetConfigString>>GadgetConfig::ListStrings()const{
	return ListSubItems<GadgetConfig,GadgetConfigString>(this,"strings");
}

std::map<std::string,std::shared_ptr<GadgetFunction>>GadgetConfig::ListLinkedFunctions()const{
	std::map<std::string,std::shared_ptr<GadgetFunction>>res;
	FolderReader d(GetPath());
	std::shared_ptr<FolderReaderItem>item;
	auto shared=GetParent();
	while((item=d.ReadItem())){
		auto fp=GetPath()+"/"+item->GetName();
		if(item->IsHidden()||!item->IsSymlink())continue;
		auto target=PathReadLink(fp);
		if(target.find("/functions/")==std::string::npos)continue;
		auto func=PathBaseName(target);
		auto val=GadgetFunction::FromName(shared,func);
		if(val)res[item->GetName()]=val;
	}
	return res;
}

std::string GadgetConfig::GetPath()const{
	auto x=gadget.lock();
	if(!x)throw RuntimeError("invalid gadget");
	return std::format("{}/configs/{}",x->GetPath(),GetName());
}

void GadgetConfig::RemoveLinkedFunction(const std::string&link_name){
	if(link_name.empty()||!CheckValid(link_name,VALIDS))
		throw RuntimeError("bad link name {}",link_name);
	RemoveFile(std::format("{}/{}",GetPath(),link_name));
}

void GadgetConfig::RemoveLinkedFunction(const std::shared_ptr<GadgetFunction>&func){
	for(const auto&kv:ListLinkedFunctions())
		if(kv.second->GetName()==func->GetName())
			RemoveLinkedFunction(kv.first);
}

void GadgetConfig::Remove(){
	for(const auto&linked:ListLinkedFunctions())
		RemoveLinkedFunction(linked.first);
	for(const auto&str:ListStrings())
		str->Remove();
	RemoveFolder(GetPath());
}

std::shared_ptr<GadgetConfigString>GadgetConfig::AddString(
	const uint16_t lang,
	const std::string&configuration
){
	auto str=std::make_shared<GadgetConfigString>(shared_from_this(),lang);
	if(!str)return nullptr;
	if(!configuration.empty())str->SetConfiguration(configuration);
	return str;
}

void GadgetConfig::LinkFunction(
	const std::shared_ptr<const GadgetFunction>&func,
	const std::string&link_name
)const{
	auto func_name=func->GetName();
	auto link_name_e=link_name.empty()?func_name:link_name;
	for(const auto&links:ListLinkedFunctions()){
		bool func_linked=links.second->GetName()==func_name;
		bool name_conflict=links.first==link_name_e;
		if(func_linked&&name_conflict)return;
		if(func_linked)throw RuntimeError("function {} already linked",func_name);
		if(name_conflict)throw RuntimeError("links {} already exists",func_name);
	}
	auto symbol_name=std::format("{}/{}",GetPath(),link_name_e);
	CreateSymlink(func->GetPath(),symbol_name);
}

std::string GadgetConfig::GetConfiguration(const uint16_t lang){
	return AddString(lang)->GetConfiguration();
}

IMPL_ATTR_HEX8(GadgetConfig,BmAttributes,bmAttributes)
IMPL_ATTR_UNSIGNED(GadgetConfig,MaxPower,MaxPower)
