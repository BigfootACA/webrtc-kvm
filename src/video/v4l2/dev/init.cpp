/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"

void V4L2Device::OnDeinitialize(){
	if(!buffers.empty()){
		v4l2_buffers_destroy(device_fd,type,memory);
		for(auto buf:buffers)delete buf;
		buffers.clear();
	}
	if(open_device&&device_fd>=0){
		close(device_fd);
		device_fd=-1;
	}
}
