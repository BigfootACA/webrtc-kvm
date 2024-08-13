/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<fcntl.h>
#include<string>
#include<sys/stat.h>
#include<sys/sysmacros.h>
#include"exception.h"
#include"strings.h"
#include"files.h"
#include"webrtc_kvm.h"

std::string GetFileProperty(const std::string&content,const std::string&key){
	if(key.empty()||content.empty())return "";
	auto token=key+"=";
	std::string::size_type off=0;
	while(true){
		auto s=content.find(token,off);
		if(s==std::string::npos)break;
		if(s!=0&&content[s-1]!='\n'){
			off=s+1;
			continue;
		}
		auto cont=content.substr(s+token.length());
		auto e=cont.find('\n');
		if(e!=std::string::npos)cont=cont.substr(0,e);
		return cont;
	}
	return "";
}

std::string DeviceGetUeventPath(mode_t type,dev_t dev){
	const char*dt;
	if(S_ISBLK(type))dt="block";
	else if(S_ISCHR(type))dt="char";
	else throw RuntimeError("bad device type");
	return std::format(
		"/sys/dev/{}/{}:{}/uevent",
		dt,major(dev),minor(dev)
	);
}

std::string DeviceReadUevent(mode_t type,dev_t dev){
	return ReadFileString(DeviceGetUeventPath(type,dev),true);
}

std::string DeviceGetUeventProperty(mode_t type,dev_t dev,const std::string&key){
	return GetFileProperty(DeviceReadUevent(type,dev),key);
}

std::string PathFromDevice(mode_t type,dev_t dev){
	struct stat st={};
	auto devname=DeviceGetUeventProperty(type,dev,"DEVNAME");
	if(devname.empty())throw RuntimeError("bad device");
	auto path="/dev/"+devname;
	if(::stat(path.c_str(),&st)!=0)
		throw ErrnoException("stat {} failed",path);
	if((st.st_mode&S_IFMT)!=(type&S_IFMT)||st.st_rdev!=dev)
		throw RuntimeError("device {} mismatch",path);
	return path;
}

int DeviceOpen(mode_t type,dev_t dev,int flags){
	auto path=PathFromDevice(type,dev);
	int fd=open(path.c_str(),flags);
	if(fd<0)throw ErrnoException("open hid device {} failed",path);
	return fd;
}

int xioctl_impl(int fd,unsigned long req,const char*name,unsigned long val){
	int ret=-1;
	do{ret=ioctl(fd,req,val);}
	while(unlikely(ret==-1&&errno==EINTR));
	if(unlikely(ret==-1))
		throw ErrnoException("ioctl {} failed",name);
	return ret;
}