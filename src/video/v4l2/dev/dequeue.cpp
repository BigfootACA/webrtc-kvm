/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"

void V4L2Device::DequeueBuffer(V4L2StreamBuffer**buffer){
	V4L2StreamBuffer*b;
	if(buffer)*buffer=nullptr;
	bool mplane=v4l2_type_mplane_check(type);
	v4l2_plane planes[VIDEO_MAX_PLANES];
	v4l2_buffer buf={.type=type,.memory=memory};
	if(mplane)buf.m.planes=planes,buf.length=plane_count;
	v4l2_buffer_dequeue(device_fd,buf);
	if(unlikely(buf.index>=buffers.size()))
		throw RuntimeError("buffer index out of bound");
	b=buffers[buf.index];
	if(unlikely(b->status!=STATUS_QUEUED))
		throw RuntimeError("buffer status mismatch");
	b->status=STATUS_DEQUEUED;
	memcpy(&b->buffer,&buf,sizeof(buf));
	if(mplane){
		memcpy(b->planes,planes,sizeof(planes));
		b->buffer.m.planes=b->planes;
	}
	if(unlikely(b->buffer.flags&V4L2_BUF_FLAG_ERROR)){
		log_trace("buffer {} got an error",buf.index);
		error++;
	}else error/=2;
	if(buffer)*buffer=b;
}
