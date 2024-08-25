/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"
#include"string/strings.h"

void V4L2Device::SetupFormat(){
	uint32_t l_width,l_height,l_fourcc;
	auto fourcc=CurrentFourcc();
	if(width==0)width=ctx->video.width;
	if(height==0)height=ctx->video.height;
	if(fps==0)fps=ctx->video.fps;
	memset(&format,0,sizeof(format));
	format.type=type;

	if(fourcc!=0&&!v4l2_pixel_format_check(device_fd,type,fourcc))
		throw InvalidArgument("missing required pixel format");

	/* Get initial info */
	if(width==0||height==0||fourcc==0){
		l_width=0,l_height=0,l_fourcc=0;
		v4l2_format_get(device_fd,format);
		v4l2_format_pixel(format,l_width,l_height,l_fourcc);
		if(width==0||height==0)l_width=width,l_height=height;
		if(fourcc==0)fourcc=l_fourcc;
	}

	/* Try required format */
	v4l2_format_setup_pixel(format,width,height,fourcc);
	v4l2_format_set(device_fd,format);

	/* Get actual format */
	v4l2_format_get(device_fd,format);
	plane_count=v4l2_format_planes_count(format);
	for(uint32_t idx_plane=0;idx_plane<plane_count;idx_plane++){
		uint32_t len=0;
		v4l2_format_sizeimage(format,idx_plane,len);
		if(input)len=MAX(len,InputPlaneSize(idx_plane));
		for(size_t i=0;i<outputs.size();i++)
			len=MAX(len,OutputPlaneSize(i,idx_plane));
		if(input)InputPlaneSize(idx_plane)=len;
		for(size_t i=0;i<outputs.size();i++)
			OutputPlaneSize(i,idx_plane)=len;
		size[idx_plane]=len;
	}
	v4l2_format_pixel(format,width,height,l_fourcc);

	log_info(
		"v4l2 {} format {}x{}@{}hz pixel {}",
		GetID(),width,height,fps,FourccToString(fourcc)
	);
}
