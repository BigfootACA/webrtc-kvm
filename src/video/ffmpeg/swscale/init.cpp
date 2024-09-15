/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"swscale.h"

void SwscalePipe::OnInitialize(){
}

void SwscalePipe::OnDeinitialize(){

}

void SwscalePipe::OnStartStream(){
	int ret;
	auto inp=GetInputStream();
	if((in_width=inp->GetWidth())==0||(in_height=inp->GetHeight())==0)
		throw InvalidArgument("no width or height");
	if(width==0)width=in_width;
	if(height==0)height=in_height;
	auto fs_in=FourccToString(fourcc_in),fs_out=FourccToString(fourcc_out);
	if((fmt_in=FourccToAVPixelFormat(fourcc_in))==AV_PIX_FMT_NONE)
		throw InvalidArgument("unsupported input fourcc {}",fs_in);
	if((fmt_out=FourccToAVPixelFormat(fourcc_out))==AV_PIX_FMT_NONE)
		throw InvalidArgument("unsupported output fourcc {}",fs_out);
	if((in_planes=av_pix_fmt_count_planes(fmt_in))<0)
		throw InvalidArgument("get input planes failed for fourcc {}: {}",fs_in,in_planes);
	if((out_planes=av_pix_fmt_count_planes(fmt_out))<0)
		throw InvalidArgument("get output planes failed for fourcc {}: {}",fs_out,out_planes);
	if(in_planes>=4)throw InvalidArgument("too many input planes");
	if(out_planes>=4)throw InvalidArgument("too many output planes");
	if(!ctx){
		if(!(sws=sws_getContext(
			(int)in_width,(int)in_height,fmt_in,
			(int)width,(int)height,fmt_out,
			SWS_BICUBIC,nullptr,nullptr,nullptr
		)))throw RuntimeError("create swscale context failed");
		log_info(
			"created swscale context {}x{} {} to {}x{} {}",
			in_width,in_height,fs_in,width,height,fs_out
		);
	}
	ptrdiff_t linesize_p[4]{};
	for(int i=0;i<out_planes;i++){
		if((ret=av_image_get_linesize(fmt_out,(int)width,i))<0)
			throw InvalidArgument("get plane {} linesize failed: {}",i,ret);
		linesize_p[i]=ret;
	}
	if((ret=av_image_fill_plane_sizes(scaled_buffer_size,fmt_out,(int)width,linesize_p))<0)
		throw InvalidArgument("get plane sizes failed: {}",ret);
	for(int i=0;i<out_planes;i++){
		if(scaled_buffer[i])av_free(scaled_buffer[i]);
		if(scaled_buffer_size[i]<=0)throw InvalidArgument("bad plane size {}",i);
		scaled_buffer[i]=(uint8_t*)av_malloc(scaled_buffer_size[i]);
		if(!scaled_buffer[i])throw RuntimeError("allocate scaled buffer for plane {} failed",i);
		log_info(
			"allocated scaled buffer {}x{} size {} for plane {}",
			width,height,SizeStringFloat(scaled_buffer_size[i]),i
		);
	}
}

void SwscalePipe::OnStopStream(){
	if(ctx){
		sws_freeContext(sws);
		sws=nullptr;
	}
	for(int i=0;i<out_planes;i++){
		scaled_buffer_size[i]=0;
		if(!scaled_buffer[i])continue;
		av_free(scaled_buffer[i]);
		scaled_buffer[i]=nullptr;
	}
	out_planes=0;
	in_planes=0;
}
