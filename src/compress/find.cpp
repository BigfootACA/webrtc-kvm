/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"compress.h"
#include"abstract/plugin.h"
#include"lib/strings.h"
#include<format>

Compressor*Compressor::LoadPlugin(const std::string&name){
	std::string real=name;
	StringReplaceAll(real,"-","_");
	auto file=std::format("compress_{}.so",real);
	auto func=std::format("webrtc_plugin_compress_get_{}",real);
	auto plugin=Plugin::Open(file);
	auto factory=plugin->Lookup<Compressor*(*)(void)>(func);
	return factory();
}

Compressor*Compressor::ByType(const std::string&type){
	for(auto c:compressor)if(c->GetType()==type)return c;
	return nullptr;
}

Compressor*Compressor::ByName(const std::string&name){
	for(auto c:compressor)if(c->IsNameSupported(name))return c;
	return LoadPlugin(name);
}

Compressor*Compressor::ByExt(const std::string&ext){
	for(auto c:compressor)if(c->IsExtSupported(ext))return c;
	return nullptr;
}

Compressor*Compressor::ByMime(const std::string&mime){
	for(auto c:compressor)if(c->IsMimeSupported(mime))return c;
	return nullptr;
}

Compressor*Compressor::ByFileName(const std::string&filename){
	auto dot=filename.rfind('.');
	if(dot==std::string::npos)return nullptr;
	return ByExt(filename.substr(dot+1));
}

Compressor*Compressor::ByContent(const Blob&data){
	for(auto c:compressor)if(c->IsCompressed(data))return c;
	return nullptr;
}
