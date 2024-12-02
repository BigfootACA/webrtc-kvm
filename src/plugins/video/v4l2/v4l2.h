/*
 * Copyright (C) 2020 Paul Kocialkowski <contact@paulk.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef V4L2_H
#define V4L2_H
#include<sys/ioctl.h>
#include<linux/videodev2.h>
#include"webrtc_kvm.h"
#include"lib/exception.h"
#include"defines.h"

header_func bool v4l2_type_mplane_check(uint32_t type){
	switch(type){
		case V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:
		case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:return true;
		default:return false;
	}
}
header_func uint32_t v4l2_type_base(uint32_t type){
	switch(type){
		case V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:return V4L2_BUF_TYPE_VIDEO_OUTPUT;
		case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:return V4L2_BUF_TYPE_VIDEO_CAPTURE;
		default:return type;
	}
}
extern void v4l2_capabilities_probe(int video_fd,uint32_t&capabilities,std::string&driver,std::string&card);
extern bool v4l2_pixel_format_check(int video_fd,uint32_t type,uint32_t pixel_format);
header_func void v4l2_format_try(int video_fd,v4l2_format&format){
	xioctl(video_fd,VIDIOC_TRY_FMT,&format);
}
header_func void v4l2_format_set(int video_fd,v4l2_format&format){
	xioctl(video_fd,VIDIOC_S_FMT,&format);
}
header_func void v4l2_format_get(int video_fd,v4l2_format&format){
	xioctl(video_fd,VIDIOC_G_FMT,&format);
}
header_func void v4l2_format_setup_pixel(v4l2_format&format,uint32_t width,uint32_t height,uint32_t pixel_format){
	if(v4l2_type_mplane_check(format.type)){
		format.fmt.pix_mp.width=width;
		format.fmt.pix_mp.height=height;
		format.fmt.pix_mp.pixelformat=pixel_format;
	}else{
		format.fmt.pix.width=width;
		format.fmt.pix.height=height;
		format.fmt.pix.pixelformat=pixel_format;
	}
}
extern void v4l2_format_setup_sizeimage(v4l2_format&format,uint32_t plane_index,uint32_t sizeimage);
extern void v4l2_format_sizeimage(v4l2_format&format,uint32_t plane_index,uint32_t&sizeimage);
extern void v4l2_format_pixel(v4l2_format&format,uint32_t&width,uint32_t&height,uint32_t&pixel_format);
header_func uint32_t v4l2_format_pixel_format(v4l2_format&format){
	return v4l2_type_mplane_check(format.type)?format.fmt.pix_mp.pixelformat:format.fmt.pix.pixelformat;
}
header_func int v4l2_format_planes_count(v4l2_format&format){
	return v4l2_type_mplane_check(format.type)?format.fmt.pix_mp.num_planes:1;
}
header_func void v4l2_selection_set(int video_fd,v4l2_selection&selection){
	xioctl(video_fd,VIDIOC_S_SELECTION,&selection);
}
header_func void v4l2_control_set(int video_fd,v4l2_control&control){
	xioctl(video_fd,VIDIOC_S_CTRL,&control);
}
header_func void v4l2_control_set_with(int video_fd,uint32_t id,int value){
	v4l2_control control={.id=id,.value=value};
	v4l2_control_set(video_fd,control);
}
header_func void v4l2_parm_set(int video_fd,v4l2_streamparm&streamparm){
	xioctl(video_fd,VIDIOC_S_PARM,&streamparm);
}
header_func void v4l2_parm_get(int video_fd,v4l2_streamparm&streamparm){
	xioctl(video_fd,VIDIOC_G_PARM,&streamparm);
}
header_func void v4l2_buffers_create(int video_fd,v4l2_create_buffers&buffer){
	xioctl(video_fd,VIDIOC_CREATE_BUFS,&buffer);
}
header_func void v4l2_buffers_request(int video_fd,uint32_t type,uint32_t memory,uint32_t count){
	v4l2_requestbuffers buffer={.count=count,.type=type,.memory=memory};
	xioctl(video_fd,VIDIOC_REQBUFS,&buffer);
}
header_func void v4l2_buffers_destroy(int video_fd,uint32_t type,uint32_t memory){
	v4l2_buffers_request(video_fd,type,memory,0);
}
header_func void v4l2_buffers_capabilities_probe(int video_fd,uint32_t type,uint32_t memory,uint32_t&capabilities){
	v4l2_format fmt={.type=type};
	v4l2_format_get(video_fd,fmt);
	v4l2_create_buffers buffer={.count=0,.memory=memory,.format=fmt};
	xioctl(video_fd,VIDIOC_CREATE_BUFS,&buffer);
	capabilities=buffer.capabilities;
}
header_func void v4l2_buffer_query(int video_fd,v4l2_buffer&buffer){
	xioctl(video_fd,VIDIOC_QUERYBUF,&buffer);
}
header_func void v4l2_buffer_queue(int video_fd,v4l2_buffer&buffer){
	xioctl(video_fd,VIDIOC_QBUF,&buffer);
}
header_func void v4l2_buffer_dequeue(int video_fd,v4l2_buffer&buffer){
	xioctl(video_fd,VIDIOC_DQBUF,&buffer);
}
header_func void v4l2_export_buffer(int video_fd,v4l2_exportbuffer&buffer){
	xioctl(video_fd,VIDIOC_EXPBUF,&buffer);
}
extern void v4l2_buffer_setup_plane_length(v4l2_buffer&buffer,uint32_t plane_index,uint32_t length);
extern void v4l2_buffer_setup_plane_length_used(v4l2_buffer&buffer,uint32_t plane_index,uint32_t length);
header_func void v4l2_buffer_setup_timestamp(v4l2_buffer&buffer,uint64_t timestamp){
	buffer.timestamp.tv_sec=timestamp/1000000000ULL;
	buffer.timestamp.tv_usec=(timestamp%1000000000ULL)/1000UL;
}
extern void v4l2_buffer_plane_offset(v4l2_buffer&buffer,uint32_t plane_index,uint32_t&offset);
extern void v4l2_buffer_plane_length(v4l2_buffer&buffer,uint32_t plane_index,uint32_t&length);
extern void v4l2_buffer_plane_length_used(v4l2_buffer&buffer,uint32_t plane_index,uint32_t&length);
header_func void v4l2_stream_on(int video_fd,uint32_t type){
	xioctl(video_fd,VIDIOC_STREAMON,&type);
}
header_func void v4l2_stream_off(int video_fd,uint32_t type){
	xioctl(video_fd,VIDIOC_STREAMOFF,&type);
}
#endif
