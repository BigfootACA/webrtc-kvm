/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/exception.h"
#include"lib/log.h"
#include"lib/strings.h"
#include"abstract/plugin.h"

struct StreamFactoryInfo{
	std::map<std::string,StreamFactory*>factories;
};

StreamFactoryInfo*StreamFactory::info;

StreamFactory*StreamFactory::LoadPlugin(const std::string&name){
	std::string real=name;
	StringReplaceAll(real,"-","_");
	auto file=std::format("video_{}.so",real);
	auto func=std::format("webrtc_plugin_video_get_{}",real);
	auto plugin=Plugin::Open(file);
	auto factory=plugin->Lookup<StreamFactory*(*)(void)>(func);
	return factory();
}

StreamFactory*StreamFactory::GetFactoryByDriverName(const std::string&driver,bool plugin){
	if(unlikely(!info||driver.empty()))return nullptr;
	auto i=info->factories.find(driver);
	if(i==info->factories.end()){
		if(plugin)try{
			return LoadPlugin(driver);
		}catch(std::exception&exc){
			log_warn("load plugin {} failed: {}",driver,exc.what());
		}
		return nullptr;
	}
	return i->second;
}

void StreamFactory::RegisterSelf(){
	driver_name=GetDriverName();
	if(info){
		auto v=info->factories.find(driver_name);
		if(v!=info->factories.end())
			throw RuntimeError("driver_name {} already exists",driver_name);
	}else info=new StreamFactoryInfo;
	info->factories[driver_name]=this;
}

void StreamFactory::UnregisterSelf(){
	if(driver_name.empty())return;
	auto v=info->factories.find(driver_name);
	if(v==info->factories.end())return;
	info->factories.erase(v);
}
