/*
 *
*Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
*SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"
#include"lib/exception.h"

size_t RockchipMediaProcessPlatform::CalcFrameSize(
	MppFrameFormat fmt,
	size_t hor_stride,
	size_t ver_stride
){
	size_t s64=round_up(hor_stride,64)*round_up(ver_stride,64);
	switch(fmt&MPP_FRAME_FMT_MASK){
		case MPP_FMT_YUV420SP:
		case MPP_FMT_YUV420P:
			return s64*3/2;
		case MPP_FMT_YUV422_YUYV:
		case MPP_FMT_YUV422_YVYU:
		case MPP_FMT_YUV422_UYVY:
		case MPP_FMT_YUV422_VYUY:
		case MPP_FMT_YUV422P:
		case MPP_FMT_YUV422SP:
			return s64*2;
		case MPP_FMT_YUV400:
		case MPP_FMT_RGB444:
		case MPP_FMT_BGR444:
		case MPP_FMT_RGB555:
		case MPP_FMT_BGR555:
		case MPP_FMT_RGB565:
		case MPP_FMT_BGR565:
		case MPP_FMT_RGB888:
		case MPP_FMT_BGR888:
		case MPP_FMT_RGB101010:
		case MPP_FMT_BGR101010:
		case MPP_FMT_ARGB8888:
		case MPP_FMT_ABGR8888:
		case MPP_FMT_BGRA8888:
		case MPP_FMT_RGBA8888:
			return s64;
		default:return s64*4;
	}
}

size_t RockchipMediaProcessPlatform::EncoderWidthDefaultStride(size_t width,MppFrameFormat fmt){
	size_t w8=round_up(width,8);
	size_t w16=round_up(width,16);
	switch(fmt&MPP_FRAME_FMT_MASK){
		case MPP_FMT_YUV400:
		case MPP_FMT_YUV420SP:
		case MPP_FMT_YUV420SP_VU:
			return w8;
		case MPP_FMT_YUV420P:
			return w16;
		case MPP_FMT_YUV422P:
		case MPP_FMT_YUV422SP:
		case MPP_FMT_YUV422SP_VU:
			return w8;
		case MPP_FMT_YUV444SP:
		case MPP_FMT_YUV444P:
			return w8;
		case MPP_FMT_RGB565:
		case MPP_FMT_BGR565:
		case MPP_FMT_RGB555:
		case MPP_FMT_BGR555:
		case MPP_FMT_RGB444:
		case MPP_FMT_BGR444:
		case MPP_FMT_YUV422_YUYV:
		case MPP_FMT_YUV422_YVYU:
		case MPP_FMT_YUV422_UYVY:
		case MPP_FMT_YUV422_VYUY:
			return w8*2;
		case MPP_FMT_RGB888:
		case MPP_FMT_BGR888:
			return w8*3;
		case MPP_FMT_RGB101010:
		case MPP_FMT_BGR101010:
		case MPP_FMT_ARGB8888:
		case MPP_FMT_ABGR8888:
		case MPP_FMT_BGRA8888:
		case MPP_FMT_RGBA8888:
			return w8*4;
		default:throw InvalidArgument("unsupported format: {}",(int)fmt);
	}
}
