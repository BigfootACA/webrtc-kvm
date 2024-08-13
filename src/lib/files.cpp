/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<cctype>
#include<string>
#include<unistd.h>
#include<cstdlib>
#include<sys/stat.h>
#include"defines.h"
#include"exception.h"
#include"full_io.h"
#include"files.h"

bool PathExists(const std::string&path){
	int ret=access(path.c_str(),F_OK);
	if(likely(ret==0))return true;
	if(likely(errno==ENOENT))return false;
	throw ErrnoException("access {} failed",path);
}

std::string PathReadLink(const std::string&path){
	char buff[PATH_MAX];
	ssize_t r=readlink(path.c_str(),buff,sizeof(buff));
	if(r<0)throw ErrnoException("readlink {} failed",path);
	return std::string(buff,r);
}

void CreateFolder(const std::string&path,mode_t mode,bool exists){
	int ret=mkdir(path.c_str(),mode);
	if(likely(ret==0))return;
	if(errno==EEXIST&&exists)return;
	throw ErrnoException("mkdir {} failed",path);
}

void RemoveFolder(const std::string&path,bool missing){
	int ret=rmdir(path.c_str());
	if(likely(ret==0))return;
	if(errno==ENOENT&&missing)return;
	if(unlikely(ret!=0))throw ErrnoException("rmdir {} failed",path);
}

void CreateSymlink(const std::string&target,const std::string&link_name){
	int ret=symlink(target.c_str(),link_name.c_str());
	if(likely(ret==0))return;
	throw ErrnoException("symlink {} -> {} failed",link_name,target);
}

void RemoveFile(const std::string&path,bool missing){
	int ret=unlink(path.c_str());
	if(likely(ret==0))return;
	if(errno==ENOENT&&missing)return;
	if(unlikely(ret!=0))throw ErrnoException("unlink {} failed",path);
}

std::string ReadFileString(const std::string&path,bool crlf){
	char*data=nullptr;
	ssize_t size=full_read_file_alloc(path.c_str(),(void**)&data);
	if(unlikely(size<0||!data))throw ErrnoException("read file {} failed",path);
	if(!crlf)while(size>0&&isspace(data[size-1]))size--;
	auto ret=std::string((const char*)data,(size_t)size);
	free(data);
	return ret;
}

int64_t ReadFileNumber(const std::string&path){
	return std::stoll(ReadFileString(path),nullptr,10);
}

uint64_t ReadFileUnsigned(const std::string&path){
	return std::stoull(ReadFileString(path),nullptr,10);
}

uint64_t ReadFileHex(const std::string&path){
	return std::stoull(ReadFileString(path),nullptr,16);
}

size_t WriteFileString(const std::string&path,const std::string&value){
	auto ret=full_write_file(path.c_str(),value.c_str(),value.size());
	if(unlikely(ret<0))throw ErrnoException("write file {} failed",path);
	return ret;
}
