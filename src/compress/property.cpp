/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<algorithm>
#include"compress.h"

std::string Compressor::GetName(){
	auto names=GetNames();
	return names.empty()?"":names[0];
}

std::string Compressor::GetMime(){
	auto mimes=GetMimes();
	return mimes.empty()?"":mimes[0];
}

std::string Compressor::GetExt(){
	auto exts=GetExts();
	return exts.empty()?"":exts[0];
}

bool Compressor::IsNameSupported(const std::string&name){
	auto names=GetNames();
	return std::find(names.begin(),names.end(),name)!=names.end();
}

bool Compressor::IsMimeSupported(const std::string&mime){
	auto mimes=GetMimes();
	return std::find(mimes.begin(),mimes.end(),mime)!=mimes.end();
}

bool Compressor::IsExtSupported(const std::string&ext){
	auto exts=GetExts();
	return std::find(exts.begin(),exts.end(),ext)!=exts.end();
}
