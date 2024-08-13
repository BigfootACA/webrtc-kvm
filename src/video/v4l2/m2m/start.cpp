/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"

void V4L2MemoryToMemory::OnStartStream(){
	v4l2_stream_on(device_fd,out->type);
	v4l2_stream_on(device_fd,cap->type);
}

void V4L2MemoryToMemory::OnStopStream(){
	DisablePoll();
	v4l2_stream_off(device_fd,out->type);
	v4l2_stream_off(device_fd,cap->type);
}
