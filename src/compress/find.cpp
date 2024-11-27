/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"compress.h"

Compressor*Compressor::ByType(const std::string&type){
	for(auto c:compressor)if(c->GetType()==type)return c;
	return nullptr;
}

Compressor*Compressor::ByName(const std::string&name){
	for(auto c:compressor)if(c->IsNameSupported(name))return c;
	return nullptr;
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
