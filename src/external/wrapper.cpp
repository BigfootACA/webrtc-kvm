/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"wrapper.h"
#include"lib/log.h"

void*LibraryLoader::GetLibrary(){
	if(unlikely(!library)){
		auto name=GetLibraryName();
		library=LoadLibrary(name);
		if(unlikely(!library))
			throw RuntimeError("load library {} failed",name);
		log_info("loaded library {}",name);
	}
	return library;
}

void*LibraryLoader::GetSymbol(const std::string&name){
	if(unlikely(!symbols[name])){
		symbols[name]=LocateSymbol(name);
		if(unlikely(!symbols[name]))
			throw LibraryError("locate symbol {} failed",name);
	}
	return symbols[name];
}

void*LibraryLoader::LoadLibrary(const std::string&name){
	auto lib=dlopen(name.c_str(),RTLD_NOW);
	if(!lib)throw LibraryError("load library {} failed: {}",name,dlerror());
	return lib;
}

void*LibraryLoader::LocateSymbol(const std::string&name){
	auto lib=GetLibrary();
	auto sym=dlsym(lib,name.c_str());
	if(!sym)throw LibraryError("load symbol {} failed: {}",name,dlerror());
	return sym;
}

LibraryLoader::~LibraryLoader(){
	if(library)dlclose(library);
	symbols.clear();
	library=nullptr;
}

#undef LibraryError
Exceptions::LibraryError::LibraryError(const std::string&msg,std::source_location c){
	this->location=c;
	this->original=msg;
	this->msg=std::format(
		"{} at {}:{}",
		msg,log::relative_filename(c),c.line()
	);
}
