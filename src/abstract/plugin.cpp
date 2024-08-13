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
#include"lib/path.h"
#include"lib/exception.h"

std::map<std::string,std::shared_ptr<Plugin>>Plugin::loaded;

Plugin::Plugin(const std::string&name){
	static std::string prefix[]={
		"",
		PathGetSelfFolder(),
		"/usr/local/lib",
		"/usr/lib",
		"/lib",
	};
	errno=ENOENT;
	for(auto path:prefix){
		if(!path.empty()&&!path.ends_with("/"))
			path+="/";
		path+=name;
		handler=dlopen(path.c_str(),RTLD_NOW);
		if(handler)return;
	}
	throw RuntimeError(
		"open plugin {} failed: {}",
		name,dlerror()?:strerror(errno)
	);
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
