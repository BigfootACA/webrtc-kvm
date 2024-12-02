/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"

void V4L2Device::InitializeBuffers(){
	use_dmabuf=false;
	if(!outputs.empty()&&GetOutput()->GetType()==BUFFER_DMABUF)use_dmabuf=true;
	bool diff_size=false;
	if(v4l2_type_mplane_check(type)){
		auto&fmt=format.fmt.pix_mp.plane_fmt;
		for(uint32_t i=0;i<plane_count;i++)
			if(size[i]!=fmt[i].sizeimage)
				diff_size=true;
	}else if(size[0]!=format.fmt.pix.sizeimage)
		diff_size=true;
	if(diff_size){
		log_info("{} plane size mismatch, use create buffer to allocate buffers",GetID());
		v4l2_create_buffers cb={.count=buffer_cnt,.memory=memory};
		memcpy(&cb.format,&format,sizeof(cb.format));
		for(uint32_t idx_plane=0;idx_plane<plane_count;idx_plane++)
			v4l2_format_setup_sizeimage(cb.format,idx_plane,size[idx_plane]);
		v4l2_buffers_create(device_fd,cb);
	}else v4l2_buffers_request(device_fd,type,memory,buffer_cnt);
	for(auto&buffer:buffers)InitializeBuffer(buffer);
}

void V4L2Device::InitializeBuffer(V4L2StreamBuffer*buffer){
	FillBuffer(buffer);
	v4l2_buffer_query(device_fd,buffer->buffer);
	switch(memory){
		case V4L2_MEMORY_MMAP:InitializeBufferMMAP(buffer);break;
		case V4L2_MEMORY_USERPTR:InitializeBufferUPTR(buffer);break;
		case V4L2_MEMORY_DMABUF:InitializeBufferDMABUF(buffer);break;
		default:throw InvalidArgument("unsupported memory type {}",(uint32_t)memory);
	}
}
