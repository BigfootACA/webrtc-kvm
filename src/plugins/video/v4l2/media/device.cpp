/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"device.h"
#include"lib/exception.h"
#include"lib/strings.h"
#include"lib/files.h"
#include"webrtc_kvm.h"
#include<sys/sysmacros.h>

MediaDevice::~MediaDevice(){
	if(auto_close&&fd>=0){
		close(fd);
		fd=-1;
	}
}

MediaDevice::MediaDevice(dev_t dev){
	fd=DeviceOpen(S_IFCHR,dev);
	auto_close=true;
	Initialize();
}

MediaDevice::MediaDevice(const std::string&path){
	if(path.empty())throw InvalidArgument("bad path");
	fd=open(path.c_str(),O_RDWR,O_CLOEXEC);
	if(fd<0)throw ErrnoException("open media device {} failed",path);
	auto_close=true;
	Initialize();
}

MediaDevice::MediaDevice(int fd,bool auto_close):fd(fd),auto_close(auto_close){
	Initialize();
}

void MediaDevice::Initialize(){
	if(fd<0)throw InvalidArgument("bad fd");
	xioctl(fd,MEDIA_IOC_DEVICE_INFO,&info);
}

std::string MediaDevice::GetDriver()const{
	return CharArrayToString(info.driver);
}

std::string MediaDevice::GetModel()const{
	return CharArrayToString(info.model);
}

std::string MediaDevice::GetSerial()const{
	return CharArrayToString(info.serial);
}

std::string MediaDevice::GetBusInfo()const{
	return CharArrayToString(info.bus_info);
}

std::string MediaDevice::ToString()const{
	return std::format(
		"Media device"
		"Driver: {}, "
		"Model: {}, "
		"Serial: {}, "
		"BusInfo: {}, "
		"Media Version: {}, "
		"Driver Version: {}, "
		"Hardware Revision: {:x}",
		NonEmpty(GetDriver()),NonEmpty(GetModel()),
		NonEmpty(GetSerial()),NonEmpty(GetBusInfo()),
		NonEmpty(LinuxVersionToString(GetMediaVersion())),
		NonEmpty(LinuxVersionToString(GetDriverVersion())),
		GetHardwareRevision()
	);
}

dev_t MediaDevice::MediaFromVideo(dev_t dev){
	auto path=std::format(
		"/sys/dev/char/{}:{}/device",
		major(dev),minor(dev)
	);
	FolderReader fr(path);
	while(auto item=fr.ReadItem())try{
		if(item->IsVirtual()||!item->IsFolder())continue;
		auto full=std::format("{}/{}",path,item->GetName());
		auto want=GetDeviceMajorMinor(full);
		if(GetDeviceSubsystem(want)!="media")continue;
		return want;
	}catch(...){}
	throw RuntimeError("no media device found for {}",DeviceToString(dev));
}

std::shared_ptr<MediaDevice>MediaDevice::FromVideoDevice(dev_t vd){
	return std::make_shared<MediaDevice>(MediaFromVideo(vd));
}

std::shared_ptr<MediaDevice>MediaDevice::FromVideoFD(int fd){
	return FromVideoDevice(DeviceFromFD(fd,S_IFCHR));
}
