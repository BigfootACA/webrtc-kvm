/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"

void V4L2Device::SetupBuffer(){
	if(!buffers.empty())throw InvalidArgument("buffers already initialized");
	for(size_t i=0;i<buffer_cnt;i++){
		auto buf=new V4L2StreamBuffer;
		buf->idx=i;
		buffers.push_back(buf);
	}
}

void V4L2Device::FillBuffer(V4L2StreamBuffer*buffer){
	memset(&buffer->buffer,0,sizeof(buffer->buffer));
	buffer->buffer.type=type;
	buffer->buffer.memory=memory;
	buffer->buffer.index=buffer->idx;
	if(v4l2_type_mplane_check(type)){
		buffer->buffer.m.planes=buffer->planes;
		buffer->buffer.length=plane_count;
	}
}
