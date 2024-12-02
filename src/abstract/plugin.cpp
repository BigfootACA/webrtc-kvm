/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<cerrno>
#include<cstring>
#include<dlfcn.h>
#include"plugin.h"
#include"lib/log.h"
#include"lib/path.h"
#include"lib/exception.h"

std::map<std::string,std::shared_ptr<Plugin>>Plugin::loaded;

Plugin::~Plugin(){
	dlclose(handler);
	log_info("unloaded plugin {}",path);
}

void Plugin::TryLoadByName(const std::string&name){
	static std::string prefix[]={
		"",
		"/"+PathMerge('/',PathGetSelfFolder(),"plugins"),
		"/usr/local/lib/webrtc-kvm",
		"/usr/lib/webrtc-kvm",
		"/lib/webrtc-kvm",
	};
	errno=ENOENT;
	for(auto path:prefix){
		if(!path.empty()&&!path.ends_with("/"))
			path+="/";
		path+=name;
		if(access(path.c_str(),F_OK)!=0)continue;
		try{
			TryLoadByPath(path);
		}catch(...){
			continue;
		}
		return;
	}
	throw RuntimeError(
		"open plugin {} failed: {}",
		name,dlerror()?:strerror(errno)
	);
}

void Plugin::TryLoadByPath(const std::string&path){
	handler=dlopen(path.c_str(),RTLD_NOW);
	if(!handler)throw RuntimeError(
		"open plugin {} failed: {}",
		name,dlerror()?:strerror(errno)
	);
	this->path=path;
	this->name=PathBaseName(name);
	log_info("loaded plugin {}",path);
}

Plugin::Plugin(const std::string&name){
	if(name.starts_with("/"))
		TryLoadByPath(name);
	else TryLoadByName(name);
}

std::shared_ptr<Plugin>Plugin::Open(const std::string&name){
	if(loaded.contains(name))return loaded[name];
	auto ptr=std::shared_ptr<Plugin>(new Plugin(name));
	loaded[name]=ptr;
	return ptr;
}

void*Plugin::LookupRAW(const std::string&sym){
	auto func=dlsym(handler,sym.c_str());
	if(!func)throw RuntimeError("symbol {} not found in {}",sym,name);
	return func;
}
