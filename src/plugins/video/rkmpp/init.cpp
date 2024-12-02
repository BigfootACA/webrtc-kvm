/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"
#include"lib/exception.h"
#include"lib/log.h"

void RockchipMediaProcessPlatform::Reset(){
	mpi=nullptr,mpp_ctx=nullptr;
	ctx_type=MPP_CTX_BUTT;
	video_type=MPP_VIDEO_CodingUnused;
	buff_fmt=MPP_FMT_BUTT;
	frm_buf=nullptr,pkt_buf=nullptr;
	buf_grp=nullptr;
	frame_size=0;
	hor_stride=0,ver_stride=0;
	enc_cfg=nullptr;
}

void RockchipMediaProcessPlatform::OnInitialize(){
	MPP_RET ret;
	auto ins=GetInputStream();
	if(width==0)width=ins->GetWidth();
	if(height==0)height=ins->GetHeight();
	if(fps==0)fps=ins->GetFrameRate();
	if(width==0)width=ctx->video.width;
	if(height==0)height=ctx->video.height;
	if(fps==0)height=ctx->video.fps;
	log_info("rkmpp {} format {}x{}@{}hz",GetID(),width,height,fps);
	if(width==0||height==0||fps==0)throw InvalidArgument("bad format");
	hor_stride=round_up(width,16),ver_stride=round_up(height,16);
	switch(role){
		case ROLE_ENCODER:EncoderPreInit();break;
		case ROLE_DECODER:DecoderPreInit();break;
		default:throw InvalidArgument("unsupported role for {}",GetID());
	}
	frame_size=CalcFrameSize(buff_fmt,hor_stride,ver_stride);
	if(MPP_ISERR(mpp_buffer_group_get_internal(&buf_grp,MPP_BUFFER_TYPE_DMA_HEAP))||!buf_grp)
		throw RuntimeError("mpp get buffer group failed: {}",(int)ret);
	if(MPP_ISERR(mpp_create(&mpp_ctx,&mpi))||!mpp_ctx||!mpi)
		throw RuntimeError("mpp create failed: {}",(int)ret);
	if(MPP_ISERR(mpp_init(mpp_ctx,ctx_type,video_type)))
		throw RuntimeError("mpp init failed: {}",(int)ret);
	switch(role){
		case ROLE_ENCODER:EncoderPostInit();break;
		case ROLE_DECODER:DecoderPostInit();break;
		default:throw InvalidArgument("unsupported role for {}",GetID());
	}
}

void RockchipMediaProcessPlatform::OnDeinitialize(){
	if(status!=STREAM_UNINITIALIZE){
		config_tempory.Clear();
		if(enc_cfg)mpp_enc_cfg_deinit(enc_cfg);
		if(mpp_ctx){
			if(mpi)mpi->reset(mpp_ctx);
			mpp_destroy(mpp_ctx);
		}
		if(buf_grp)mpp_buffer_group_put(buf_grp);
		if(pkt_buf)mpp_buffer_put(pkt_buf);
		if(frm_buf)mpp_buffer_put(frm_buf);
	}
	Reset();
}
