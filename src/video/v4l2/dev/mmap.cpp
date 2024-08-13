/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"
#include"lib/readable.h"

void V4L2Device::InitializeBufferMMAP(V4L2StreamBuffer*buffer){
	for(uint32_t idx_plane=0;idx_plane<plane_count;idx_plane++){
		uint32_t offset=0,length=0;
		v4l2_buffer_plane_offset(buffer->buffer,idx_plane,offset);
		v4l2_buffer_plane_length(buffer->buffer,idx_plane,length);
		void*ptr=mmap(nullptr,length,PROT_READ|PROT_WRITE,MAP_SHARED,device_fd,offset);
		if(unlikely(!ptr||ptr==MAP_FAILED))throw ErrnoException("mmap failed");
		buffer->mmap_data[idx_plane]=ptr;
		log_dbg(
			"v4l2 {} mapped {}.{} buffer offset 0x{:X} size {}",
			GetID(),buffer->idx,idx_plane,offset,SizeStringFloat(length)
		);
		if(use_dmabuf){
			v4l2_exportbuffer expbuf={
				.type=type,
				.index=buffer->idx,
				.plane=idx_plane,
				.flags=O_RDWR,
				.fd=-1,
			};
			v4l2_export_buffer(device_fd,expbuf);
			buffer->dmabuf_fds[idx_plane]=expbuf.fd;
			log_dbg(
				"v4l2 {} export {}.{} dmabuf fd {}",
				GetID(),buffer->idx,idx_plane,expbuf.fd
			);
			auto name=std::format(
				"dev{}-buf{}-plane{}",
				expbuf.fd,buffer->idx,idx_plane
			);
			xioctl(expbuf.fd,DMA_BUF_SET_NAME,name.c_str());
		}else buffer->dmabuf_fds[idx_plane]=-1;
	}
}
