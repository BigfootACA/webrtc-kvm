/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"swscale.h"

void SwscalePipe::OnProcessInput(StreamBuffer*buffer){
	int ret;
	uint8_t*data[4]{};
	int linesizes_in[4]{},linesizes_out[4]{};
	if(unlikely(buffer->type!=BUFFER_POINTER))
		throw InvalidArgument("unsupported buffer type");
	if((ret=av_image_fill_linesizes(linesizes_in,fmt_in,(int)in_width))<0)
		throw RuntimeError("fill source image linesize failed: {}",ret);
	if((ret=av_image_fill_linesizes(linesizes_out,fmt_out,(int)width))<0)
		throw RuntimeError("fill scaled image linesize failed: {}",ret);
	if(buffer->planes_cnt==1&&in_planes>1){
		ret=av_image_fill_pointers(
			data,fmt_in,(int)in_height,
			(uint8_t*)buffer->planes[0].ptr,
			linesizes_in
		);
		if(ret<0)throw RuntimeError("fill source image pointers failed: {}",ret);
	}else if((int)buffer->planes_cnt==in_planes)
		for(uint32_t i=0;i<buffer->planes_cnt;i++)
			data[i]=(uint8_t*)buffer->planes[i].ptr;
	else throw InvalidArgument("plane count mismatch");
	if(unlikely((ret=sws_scale(
		sws,data,linesizes_in,0,in_height,
		scaled_buffer,linesizes_out
	))<0))throw RuntimeError("scale failed: {}",ret);
	StreamBuffer buff{.type=BUFFER_POINTER};
	buff.planes_cnt=out_planes;
	buff.timestamp=buffer->timestamp;
	for(int i=0;i<out_planes;i++){
		buff.planes[i].size=scaled_buffer_size[i];
		buff.planes[i].used=scaled_buffer_size[i];
		buff.planes[i].ptr=scaled_buffer[i];
	}
	SendToNext(&buff);
}