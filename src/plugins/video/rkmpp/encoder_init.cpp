/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"
#include"lib/exception.h"

void RockchipMediaProcessPlatform::EncoderPreInit(){
	MPP_RET ret;
	if(MPP_ISERR(mpp_enc_cfg_init(&enc_cfg)))
		throw RuntimeError("mpp init encoder config failed: {}",(int)ret);
	video_type=FourccToMppCodingType(fourcc_out);
	buff_fmt=FourccToMppFrameFormat(fourcc_in);
	hor_stride=EncoderWidthDefaultStride(width,buff_fmt);
	config_tempory.AddS32("tune:scene_mode",MPP_ENC_SCENE_MODE_DEFAULT);
        config_tempory.AddS32("prep:width",width);
        config_tempory.AddS32("prep:height",height);
        config_tempory.AddS32("prep:hor_stride",hor_stride);
        config_tempory.AddS32("prep:ver_stride",ver_stride);
        config_tempory.AddS32("prep:format",buff_fmt);
        config_tempory.AddS32("rc:mode",MPP_ENC_RC_MODE_CBR);
        config_tempory.AddS32("rc:fps_in_flex",0);
        config_tempory.AddS32("rc:fps_in_num",fps);
        config_tempory.AddS32("rc:fps_in_denom",1);
        config_tempory.AddS32("rc:fps_out_flex",0);
        config_tempory.AddS32("rc:fps_out_num",fps);
        config_tempory.AddS32("rc:fps_out_denom",1);
	config_tempory.AddS32("codec:type",video_type);
	ctx_type=MPP_CTX_ENC;
}

void RockchipMediaProcessPlatform::EncoderPostInit(){
	MPP_RET ret;
	if(MPP_ISERR(mpi->control(mpp_ctx,MPP_ENC_GET_CFG,enc_cfg)))
		throw RuntimeError("mpp get encoder config failed: {}",(int)ret);
	config_tempory.FillToEncoder(enc_cfg);
	config_persist.FillToEncoder(enc_cfg);
	if(MPP_ISERR(mpi->control(mpp_ctx,MPP_ENC_SET_CFG,enc_cfg)))
		throw RuntimeError("mpp set encoder config failed: {}",(int)ret);
	if(video_type==MPP_VIDEO_CodingAVC||video_type==MPP_VIDEO_CodingHEVC){
		auto sei_mode=MPP_ENC_SEI_MODE_DISABLE;
		if(MPP_ISERR(mpi->control(mpp_ctx,MPP_ENC_SET_SEI_CFG,&sei_mode)))
			throw RuntimeError("mpp set encoder sei config failed: {}",(int)ret);
		auto header_mode=MPP_ENC_HEADER_MODE_EACH_IDR;
		if(MPP_ISERR(mpi->control(mpp_ctx,MPP_ENC_SET_HEADER_MODE,&header_mode)))
			throw RuntimeError("mpp set encoder header mode failed: {}",(int)ret);
	}
	if(MPP_ISERR(mpp_frame_init(&enc_frm)))
		throw RuntimeError("mpp init frame failed: {}",(int)ret);
	if(MPP_ISERR(mpp_buffer_get(buf_grp,&pkt_buf,frame_size))||!pkt_buf)
		throw RuntimeError("mpp get packet buffer failed: {}",(int)ret);
	if(GetInput()->GetType()==BUFFER_POINTER){
		if(MPP_ISERR(mpp_buffer_get(buf_grp,&frm_buf,frame_size))||!frm_buf)
			throw RuntimeError("mpp get frame buffer failed: {}",(int)ret);
	}
}
