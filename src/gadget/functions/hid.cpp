/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../core/internal.h"
#include"lib/full_io.h"
#include"lib/path.h"
#include"lib/readable.h"
#include"lib/exception.h"
#include"gadget_hid.h"
#include<sys/stat.h>
#include<sys/sysmacros.h>

IMPL_ATTR_UNSIGNED(GadgetHID,Protocol,protocol)
IMPL_ATTR_UNSIGNED(GadgetHID,SubClass,subclass)
IMPL_ATTR_UNSIGNED(GadgetHID,ReportLength,report_length)

void GadgetHID::WriteReportDescriptor(const HIDReportDesc*desc){
	if(!desc||!desc->data||desc->size<=0)
		throw RuntimeError("bad hid descriptor");
	auto path=std::format("{}/report_desc",GetPath());
	log_dbg("write {} hid report descriptor",SizeStringFloat(desc->size));
	if(full_write_file(path.c_str(),desc->data,desc->size)!=(ssize_t)desc->size)
		throw ErrnoException("write hid descriptor {} failed",path);
}

dev_t GadgetHID::GetDevice(){
	auto d=GetDeviceMajorMinor(GetPath());
	if(GetDeviceSubsystem(d)!="hidg")
		throw RuntimeError("target hid dev is not a hidg");
	return d;
}

std::string GadgetHID::GetDevicePath(){
	return PathFromDevice(S_IFCHR,GetDevice());
}

int GadgetHID::OpenDevice(int flags){
	auto r=DeviceOpen(S_IFCHR,GetDevice(),flags);
	if(r>=0)log_dbg("open HID gadget {} as fd {}",GetName(),r);
	return r;
}
