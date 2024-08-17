/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_cap.h"


void V4L2Capture::ProcessNextInput(V4L2StreamBuffer*in){
	if(in->status!=STATUS_DEQUEUED)return;
	StreamBuffer out={
		.type=output->GetType(),
		.planes_cnt=plane_count,
		.timestamp=in->buffer.timestamp,
	};
	uint32_t val=0;
	for(uint32_t i=0;i<plane_count;i++){
		switch(out.type){
			case BUFFER_DMABUF:out.planes[i].fd=in->dmabuf_fds[i];break;
			case BUFFER_POINTER:out.planes[i].ptr=in->mmap_data[i];break;
			default:throw InvalidArgument("unsupported buffer type");
		}
		v4l2_buffer_plane_length(in->buffer,i,val);
		out.planes[i].size=val;
		v4l2_buffer_plane_length_used(in->buffer,i,val);
		out.planes[i].used=val;
	}
	SendToNext(&out);
	in->status=STATUS_IDLE;
}

void V4L2Capture::OnProcessNeedQueue(){
	for(auto buf:buffers)switch(buf->status){
		case STATUS_DEQUEUED:
			ProcessNextInput(buf);
		break;
		case STATUS_IDLE:
		case STATUS_PREPARED:
			QueueBuffer(buf);
		break;
		default:;
	}
}

void V4L2Capture::OnProcessOutput(){
	V4L2StreamBuffer*buffer=nullptr;
	DequeueBuffer(&buffer);
	ProcessNextInput(buffer);
	QueueBuffer(buffer);
}
