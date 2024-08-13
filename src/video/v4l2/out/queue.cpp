/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_out.h"

void V4L2Output::PrepareInput(StreamBuffer*input,V4L2StreamBuffer*buffer){
	if(buffer->status!=STATUS_IDLE)return;
	FillBuffer(buffer);
	buffer->buffer.timestamp=input->timestamp;
	for(uint32_t idx_plane=0;idx_plane<input->planes_cnt;idx_plane++){
		v4l2_buffer_setup_plane_length(buffer->buffer,idx_plane,input->planes[idx_plane].size);
		v4l2_buffer_setup_plane_length_used(buffer->buffer,idx_plane,input->planes[idx_plane].used);
	}
	switch(input->type){
		case BUFFER_DMABUF:
			if(unlikely(memory!=V4L2_MEMORY_DMABUF))
				throw RuntimeError("request dmabuf but not in dmabuf");
			if(!v4l2_type_mplane_check(type))
				buffer->buffer.m.fd=input->planes[0].fd;
			else for(uint32_t idx_plane=0;idx_plane<input->planes_cnt;idx_plane++)
				buffer->planes[idx_plane].m.fd=input->planes[idx_plane].fd;
		break;
		case BUFFER_POINTER:
			if(unlikely(memory!=V4L2_MEMORY_MMAP&&memory!=V4L2_MEMORY_USERPTR))
				throw RuntimeError("request pointer but not in mmap or userptr");
			for(uint32_t idx_plane=0;idx_plane<input->planes_cnt;idx_plane++){
				size_t len=input->planes[idx_plane].used;
				memcpy(buffer->mmap_data[idx_plane],input->planes[idx_plane].ptr,len);
			}
		break;
		default:throw RuntimeError("unsupported stream buffer type");
	}
	buffer->status=STATUS_PREPARED;
}

void V4L2Output::QueueInput(StreamBuffer*input,V4L2StreamBuffer*buffer){
	PrepareInput(input,buffer);
	QueueBuffer(buffer);
}
