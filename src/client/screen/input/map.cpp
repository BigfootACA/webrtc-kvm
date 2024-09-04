/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"input.h"
#include"lib/exception.h"

struct InputKeyMapFactoryContext{
	std::vector<InputKeyMapFactory*>v;
};

static InputKeyMapFactoryContext*factories=nullptr;

void InputKeyMapFactory::RegisterSelf(){
	if(!factories)factories=new InputKeyMapFactoryContext;
	factories->v.push_back(this);
}

void InputKeyMapFactory::UnregisterSelf(){
	if(!factories)return;
	auto idx=std::find(factories->v.begin(),factories->v.end(),this);
	if(idx==factories->v.end())return;
	factories->v.erase(idx);
	if(factories->v.empty()){
		delete factories;
		factories=nullptr;
	}
}

InputKeyMapFactory*InputKeyMapFactory::FindFactoryByName(const std::string&name){
	if(!factories)return nullptr;
	for(auto f:factories->v)
		if(f->GetName()==name)
			return f;
	return nullptr;
}

InputKeyMapFactory*InputKeyMapFactory::FindFactoryBySupported(const std::string&plugin){
	if(!factories)return nullptr;
	for(auto f:factories->v){
		auto s=f->GetSupported();
		auto i=std::find(s.begin(),s.end(),plugin);
		if(i!=s.end())return f;
	}
	return nullptr;
}

std::shared_ptr<InputKeyMap>InputKeyMapFactory::CreateByName(const std::string&name){
	auto f=FindFactoryByName(name);
	return f?f->Create():nullptr;
}

std::shared_ptr<InputKeyMap>InputKeyMapFactory::CreateBySupported(const std::string&plugin){
	auto f=FindFactoryBySupported(plugin);
	return f?f->Create():nullptr;
}

std::shared_ptr<InputKeyMap>InputKeyMapFactory::CreateAuto(){
	auto platform=QGuiApplication::platformName().toStdString();
	log_info("qt platform: {}",platform);
	auto map=FindFactoryBySupported(platform);
	if(!map)map=FindFactoryByName("qt-fallback");
	if(!map)throw InvalidArgument("no any key map found");
	log_info("use key map {}",map->GetName());
	return map->Create();
}
