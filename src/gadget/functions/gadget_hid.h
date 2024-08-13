/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_HID_H
#define GADGET_HID_H
#include<fcntl.h>
#include<stdlib.h>
#include"../core/gadget_function.h"
#include"input/hid.h"

class GadgetHID:public GadgetFunction{
	public:
		GadgetHID(std::weak_ptr<const Gadget>gadget,const std::string&id);
		DECL_ATTR(Protocol,uint64_t);
		DECL_ATTR(SubClass,uint64_t);
		DECL_ATTR(ReportLength,uint64_t);
		void WriteReportDescriptor(const HIDReportDesc*desc);
		dev_t GetDevice();
		std::string GetDevicePath();
		int OpenDevice(int flags=O_RDWR|O_CLOEXEC);
};

#endif
