/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"video.h"

bool VideoProcess::receiveFrame(){
	int ret;
	ret=avcodec_receive_frame(ctx,decoded_frame);
	if(ret==AVERROR(EAGAIN)||ret==AVERROR_EOF)return false;
	if(unlikely(ret<0))throw ErrnoExceptionWith(
		AVUNERROR(ret),
		"avcodec receive frame failed"
	);
	auto frame=decoded_frame;
	VideoFormat src={
		.w=decoded_frame->width,
		.h=decoded_frame->height,
		.f=(AVPixelFormat)decoded_frame->format,
	};
	VideoFormat dst=scaled;
	auto fmt=AVPixelFormatToQImageFormat((AVPixelFormat)decoded_frame->format);
	if(fmt==QImage::Format::Format_Invalid||src.w!=dst.w||src.h!=dst.h){
		if(unlikely(src!=last.src||dst!=last.dst)){
			log_info("stream resize detected");
			log_info("video size {}x{} -> {}x{}",last.src.w,last.src.h,src.w,src.h);
			log_info("display size {}x{} -> {}x{}",last.dst.w,last.dst.h,dst.w,dst.h);
			if(dst.w==0)dst.w=src.w;
			if(dst.h==0)dst.h=src.h;
			last.src=src,last.dst=dst;
			scaled_frame->width=dst.w;
			scaled_frame->height=dst.h;
			scaled_frame->format=dst.f;
			destroyScaler();
		}
		if(unlikely(!sws||!scaled_buffer))initScaler(src,dst);
		frame=scaled_frame;
		ret=sws_scale(
			sws,decoded_frame->data,decoded_frame->linesize,
			0,src.h,scaled_frame->data,scaled_frame->linesize
		);
		if(unlikely(ret<0))throw RuntimeError("sws_scale failed");
	}
	screen->current.frames+=1;
	commitFrame(frame);
	return true;
}
