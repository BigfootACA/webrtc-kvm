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

void RockchipMediaProcessPlatform::WriteFrame(MppFrame frame){
	if(unlikely(!frame))return;
	StreamBuffer buff={
		.type=GetOutput()->GetType(),
		.planes_cnt=1,
	};
	TimestampFromFrames(buff.timestamp,frames);
	auto buffer=mpp_frame_get_buffer(frame);
	if(unlikely(!buffer))throw InvalidArgument("no buffer in frame");
	buff.planes[0].used=frame_size;
	buff.planes[0].size=mpp_buffer_get_size(buffer);
	buff.planes[0].ptr=mpp_buffer_get_ptr(buffer);
	SendToNext(&buff);
}

void RockchipMediaProcessPlatform::OnProcessInputDecoder(MppBuffer buf,StreamBuffer*buffer){
	MPP_RET ret;
	MppFrame frame=nullptr;
	auto ts=frames*1000/fps;
	mpp_packet_set_dts(dec_pkt,ts);
	mpp_packet_set_pts(dec_pkt,ts);
	mpp_packet_set_buffer(dec_pkt,buf);
	if(MPP_ISERR(mpi->decode_put_packet(mpp_ctx,dec_pkt)))
		throw RuntimeError("mpp put packet failed: {}",(int)ret);
	if(MPP_ISERR(mpi->decode_get_frame(mpp_ctx,&frame)))
		throw RuntimeError("mpp get frame failed: {}",(int)ret);
	ResourceRelease<MppFrame>rp(frame,[](MppFrame p){
		mpp_frame_deinit(&p);
	});
	WriteFrame(frame);
}
