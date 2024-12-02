/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"

void V4L2Device::QueueBuffer(V4L2StreamBuffer*buffer){
	if(buffer->status==STATUS_DEQUEUED)return;
	if(buffer->status==STATUS_IDLE){
		FillBuffer(buffer);
		buffer->status=STATUS_PREPARED;
	}
	if(buffer->status==STATUS_PREPARED){
		v4l2_buffer_queue(device_fd,buffer->buffer);
		buffer->status=STATUS_QUEUED;
	}
	if(unlikely(buffer->status!=STATUS_QUEUED))
		throw RuntimeError("bad buffer status {} for queue",(int)buffer->status);
}
