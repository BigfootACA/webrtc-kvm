/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/log.h"
#include"lib/exception.h"

void Stream::OpenDevice(){
	if(!open_device)throw InvalidArgument("device does not initialized");
	if(path.empty())throw InvalidArgument("no device set for {}",GetID());
	if((device_fd=open(path.c_str(),O_RDWR|O_NONBLOCK|O_CLOEXEC))<0)
		throw ErrnoException("open device {} failed",path);
	log_dbg("stream {} open device {} as {}",GetID(),path,device_fd);
}
