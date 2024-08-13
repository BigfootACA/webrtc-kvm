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
		for(auto buf:buffers){
			for(uint32_t i=0;i<plane_count;i++){
				uint32_t len=0;
				v4l2_buffer_plane_length(buf->buffer,i,len);
				if(buf->dmabuf_fds[i]>0)
					close(buf->dmabuf_fds[i]);
				if(buf->mmap_data[i]&&buf->mmap_data[i]!=MAP_FAILED)
					munmap(buf->mmap_data[i],len);
			}
			delete buf;
		}
		buffers.clear();
	}
	if(open_device&&device_fd>=0){
		close(device_fd);
		device_fd=-1;
	}
}
