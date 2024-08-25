/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"
#include"lib/exception.h"
#include"string/strings.h"
#include<linux/videodev2.h>

auto x=MPP_VIDEO_CodingAVC;

uint32_t RockchipMediaProcessPlatform::MppCodingTypeToFourcc(MppCodingType type){
	switch(type){
		case MPP_VIDEO_CodingMPEG2:return V4L2_PIX_FMT_MPEG2;
		case MPP_VIDEO_CodingH263:return V4L2_PIX_FMT_H263;
		case MPP_VIDEO_CodingMPEG4:return V4L2_PIX_FMT_MPEG4;
		case MPP_VIDEO_CodingAVC:return V4L2_PIX_FMT_H264;
		case MPP_VIDEO_CodingMJPEG:return V4L2_PIX_FMT_MJPEG;
		case MPP_VIDEO_CodingVP8:return V4L2_PIX_FMT_VP8;
		case MPP_VIDEO_CodingVP9:return V4L2_PIX_FMT_VP9;
		case MPP_VIDEO_CodingHEVC:return V4L2_PIX_FMT_HEVC;
		default:throw InvalidArgument("unsupported type: {}",(int)type);
	}
}

MppCodingType RockchipMediaProcessPlatform::FourccToMppCodingType(uint32_t fourcc){
	switch(fourcc){
		case V4L2_PIX_FMT_MPEG2:return MPP_VIDEO_CodingMPEG2;
		case V4L2_PIX_FMT_H263:return MPP_VIDEO_CodingH263;
		case V4L2_PIX_FMT_MPEG4:return MPP_VIDEO_CodingMPEG4;
		case V4L2_PIX_FMT_H264:return MPP_VIDEO_CodingAVC;
		case V4L2_PIX_FMT_MJPEG:return MPP_VIDEO_CodingMJPEG;
		case V4L2_PIX_FMT_VP8:return MPP_VIDEO_CodingVP8;
		case V4L2_PIX_FMT_VP9:return MPP_VIDEO_CodingVP9;
		case V4L2_PIX_FMT_HEVC:return MPP_VIDEO_CodingHEVC;
		default:throw InvalidArgument("unsupported fourcc: {}",FourccToString(fourcc));
	}
}

uint32_t RockchipMediaProcessPlatform::MppFrameFormatToFourcc(MppFrameFormat fmt){
	switch(fmt){
		case MPP_FMT_YUV420SP:return V4L2_PIX_FMT_NV12;
		case MPP_FMT_YUV422SP:return V4L2_PIX_FMT_NV16;
		case MPP_FMT_YUV422P:return V4L2_PIX_FMT_YUV422P;
		case MPP_FMT_YUV422_YUYV:return V4L2_PIX_FMT_YUYV;
		case MPP_FMT_YUV422_YVYU:return V4L2_PIX_FMT_YVYU;
		case MPP_FMT_YUV422_UYVY:return V4L2_PIX_FMT_UYVY;
		case MPP_FMT_YUV422_VYUY:return V4L2_PIX_FMT_VYUY;
		case MPP_FMT_RGB565:return V4L2_PIX_FMT_RGB565;
		case MPP_FMT_RGB555:return V4L2_PIX_FMT_RGB555;
		case MPP_FMT_RGB444:return V4L2_PIX_FMT_RGB444;
		case MPP_FMT_RGB888:return V4L2_PIX_FMT_RGB24;
		case MPP_FMT_BGR888:return V4L2_PIX_FMT_BGR24;
		case MPP_FMT_ARGB8888:return V4L2_PIX_FMT_ARGB32;
		case MPP_FMT_ABGR8888:return V4L2_PIX_FMT_ABGR32;
		case MPP_FMT_BGRA8888:return V4L2_PIX_FMT_BGRA32;
		case MPP_FMT_RGBA8888:return V4L2_PIX_FMT_RGBA32;
		default:throw InvalidArgument("unsupported format: {}",(int)fmt);
	}
}

MppFrameFormat RockchipMediaProcessPlatform::FourccToMppFrameFormat(uint32_t fourcc){
	switch(fourcc){
		case V4L2_PIX_FMT_NV12:return MPP_FMT_YUV420SP;
		case V4L2_PIX_FMT_NV16:return MPP_FMT_YUV422SP;
		case V4L2_PIX_FMT_YUV422P:return MPP_FMT_YUV422P;
		case V4L2_PIX_FMT_YUYV:return MPP_FMT_YUV422_YUYV;
		case V4L2_PIX_FMT_YVYU:return MPP_FMT_YUV422_YVYU;
		case V4L2_PIX_FMT_UYVY:return MPP_FMT_YUV422_UYVY;
		case V4L2_PIX_FMT_VYUY:return MPP_FMT_YUV422_VYUY;
		case V4L2_PIX_FMT_RGB565:return MPP_FMT_RGB565;
		case V4L2_PIX_FMT_RGB555:return MPP_FMT_RGB555;
		case V4L2_PIX_FMT_RGB444:return MPP_FMT_RGB444;
		case V4L2_PIX_FMT_RGB24:return MPP_FMT_RGB888;
		case V4L2_PIX_FMT_BGR24:return MPP_FMT_BGR888;
		case V4L2_PIX_FMT_ARGB32:return MPP_FMT_ARGB8888;
		case V4L2_PIX_FMT_ABGR32:return MPP_FMT_ABGR8888;
		case V4L2_PIX_FMT_BGRA32:return MPP_FMT_BGRA8888;
		case V4L2_PIX_FMT_RGBA32:return MPP_FMT_RGBA8888;
		default:throw InvalidArgument("unsupported fourcc: {}",FourccToString(fourcc));
	}
}
