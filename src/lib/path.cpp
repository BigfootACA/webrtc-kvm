/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<climits>
#include"path.h"

std::string PathResolve(char sep,const std::string&path){
	char buff[PATH_MAX];
	size_t r=::path_resolve(
		buff,sizeof(buff),
		sep,path.c_str()
	);
	return std::string(buff,r);
}

std::string PathMerge(char sep,const std::string&path1,const std::string&path2){
	char buff[PATH_MAX];
	size_t r=::path_merge(
		buff,sizeof(buff),sep,
		path1.c_str(),path2.c_str()
	);
	return std::string(buff,r);
}

std::string PathBaseName(const std::string&path){
	char buff[PATH_MAX];
	return ::path_basename(
		buff,sizeof(buff),
		path.c_str(),path.length()
	)?std::string(buff):"";
}

std::string PathDirName(const std::string&path){
	char buff[PATH_MAX];
	return ::path_dirname(
		buff,sizeof(buff),
		path.c_str(),path.length()
	)?std::string(buff):"";
}

std::string PathGetSelf(){
	char buff[PATH_MAX];
	return ::path_get_self(buff,sizeof(buff))?std::string(buff):"";
}

std::string PathGetSelfFolder(){
	char buff[PATH_MAX];
	return ::path_get_self_folder(buff,sizeof(buff))?std::string(buff):"";
}
