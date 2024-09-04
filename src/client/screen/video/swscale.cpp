/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"video.h"
#include"lib/readable.h"

void VideoProcess::initScaledFrame(const VideoFormat&dst){
	int ret;
	if(likely(scaled_buffer))return;
	auto alg=sizeof(void*);
	scaled_buffer_size=av_image_get_buffer_size(dst.f,dst.w,dst.h,alg);
	scaled_buffer=(uint8_t*)av_malloc(scaled_buffer_size);
	if(!scaled_buffer)throw RuntimeError("allocate buffer failed");
	log_info(
		"scaled buffer {}x{} size {}",
		dst.w,dst.h,SizeStringFloat(scaled_buffer_size)
	);
	ret=av_image_fill_arrays(
		scaled_frame->data,scaled_frame->linesize,
		scaled_buffer,dst.f,dst.w,dst.h,alg
	);
	if(ret<0)throw RuntimeError("av_image_fill_arrays failed");
}

void VideoProcess::initScaler(const VideoFormat&src,const VideoFormat&dst){
	if(unlikely(!sws)){
		if(!(sws=sws_getContext(
			src.w,src.h,src.f,dst.w,dst.h,dst.f,
			SWS_BICUBIC,nullptr,nullptr,nullptr
		)))throw RuntimeError("create swscale context failed");
		log_info("swscale created with {}x{} -> {}x{}",src.w,src.h,dst.w,dst.h);
	}
	initScaledFrame(dst);
}
