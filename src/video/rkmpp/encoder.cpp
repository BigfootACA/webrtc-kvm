/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"
#include"lib/exception.h"
#include"abstract/resource_release.h"

void RockchipMediaProcessPlatform::WritePacket(MppPacket packet){
	if(unlikely(!packet))return;
	StreamBuffer buff={
		.type=GetOutput()->GetType(),
		.planes_cnt=1,
	};
	TimestampFromFrames(buff.timestamp,frames);
	buff.planes[0].size=mpp_packet_get_size(packet);
	buff.planes[0].used=mpp_packet_get_length(packet);
	buff.planes[0].ptr=mpp_packet_get_pos(packet);
	SendToNext(&buff);
}

void RockchipMediaProcessPlatform::WriteHeader(){
	MPP_RET ret;
	MppPacket packet=nullptr;
	if(video_type!=MPP_VIDEO_CodingAVC&&video_type!=MPP_VIDEO_CodingHEVC)return;
	mpp_packet_init_with_buffer(&packet,pkt_buf);
	mpp_packet_set_length(packet,0);
	ResourceRelease<MppPacket>rp(packet,[](MppPacket p){mpp_packet_deinit(&p);});
	if(MPP_ISERR(mpi->control(mpp_ctx,MPP_ENC_GET_HDR_SYNC,packet)))
		throw RuntimeError("mpp get header failed: {}",(int)ret);
	sps_pkt=rp.Take();
}

void RockchipMediaProcessPlatform::OnProcessInputEncoder(MppBuffer buf,StreamBuffer*buffer){
	MPP_RET ret;
	MppPacket packet=nullptr;
	auto ts=frames*1000/fps;
	mpp_frame_set_dts(enc_frm,ts);
	mpp_frame_set_pts(enc_frm,ts);
	mpp_frame_set_buffer(enc_frm,buf);
	mpp_frame_set_width(enc_frm,width);
	mpp_frame_set_height(enc_frm,height);
	mpp_frame_set_hor_stride(enc_frm,hor_stride);
	mpp_frame_set_ver_stride(enc_frm,ver_stride);
	mpp_frame_set_fmt(enc_frm,buff_fmt);
	if(MPP_ISERR(mpi->encode_put_frame(mpp_ctx,enc_frm)))
		throw RuntimeError("mpp put frame failed: {}",(int)ret);
	if(MPP_ISERR(mpi->encode_get_packet(mpp_ctx,&packet)))
		throw RuntimeError("mpp get packet failed: {}",(int)ret);
	ResourceRelease<MppPacket>rp(packet,[](MppPacket p){
		mpp_packet_deinit(&p);
	});
	WritePacket(packet);
}
