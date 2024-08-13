/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"
#include"config/configs.h"

void V4L2MemoryToMemory::LoadH264EncoderConfig(YAML::Node obj){
	if(!obj)return;
	if(auto v=obj["qp_i"])options_h264.qp_i=v.as<uint32_t>();
	if(auto v=obj["qp_p"])options_h264.qp_p=v.as<uint32_t>();
	if(auto v=obj["gop_size"])options_h264.gop_size=v.as<uint32_t>();
	if(auto v=obj["gop_closure"])options_h264.gop_closure=v.as<bool>();
	load_string_conv(obj,level,options_h264.level,H264Level);
	load_string_conv(obj,profile,options_h264.profile,H264Profile);
	#define SET_CTRL(_id,_val) controls[(uint32_t)(_id)]=(int32_t)(_val)
	SET_CTRL(V4L2_CID_MPEG_VIDEO_PREPEND_SPSPPS_TO_IDR,1);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_H264_I_FRAME_QP,options_h264.qp_i);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_H264_P_FRAME_QP,options_h264.qp_p);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_H264_LEVEL,V4L2_MPEG_VIDEO_H264_LEVEL_5_0);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_H264_PROFILE,V4L2_MPEG_VIDEO_H264_PROFILE_CONSTRAINED_BASELINE);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_H264_ENTROPY_MODE,V4L2_MPEG_VIDEO_H264_ENTROPY_MODE_CAVLC);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_GOP_CLOSURE,options_h264.gop_closure);
	SET_CTRL(options_h264.gop_closure?
	         V4L2_CID_MPEG_VIDEO_GOP_SIZE:
	         V4L2_CID_MPEG_VIDEO_H264_I_PERIOD,
	         options_h264.gop_size
	);
	#undef SET_CTRL
}
