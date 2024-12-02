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

#include <fcntl.h>
#include <unistd.h>
#include "v4l2.h"

void v4l2_capabilities_probe(int video_fd,uint32_t&capabilities,std::string&driver,std::string&card){
	struct v4l2_capability capability={};
	xioctl(video_fd,VIDIOC_QUERYCAP,&capability);
	capabilities=(capability.capabilities&V4L2_CAP_DEVICE_CAPS)?capability.device_caps:capability.capabilities;
	driver=(char*)capability.driver;
	card=(char*)capability.card;
}

bool v4l2_pixel_format_check(int video_fd,uint32_t type,uint32_t pixel_format){
	uint32_t index=0;
	while(true)try{
		struct v4l2_fmtdesc fmtdesc={.index=index,.type=type};
		xioctl(video_fd,VIDIOC_ENUM_FMT,&fmtdesc);
		if(fmtdesc.pixelformat==pixel_format)return true;
		index++;
	}catch(Exceptions::ErrnoExceptionImpl&exc){
		if(exc.err==EAGAIN)continue;
		break;
	}
	return false;
}

void v4l2_format_setup_sizeimage(v4l2_format&format,uint32_t plane_index,uint32_t sizeimage){
	if(v4l2_type_mplane_check(format.type)){
		if(unlikely(plane_index>=format.fmt.pix_mp.num_planes))
			throw InvalidArgument("bad plane index");
		format.fmt.pix_mp.plane_fmt[plane_index].sizeimage=sizeimage;
	}else{
		if(unlikely(plane_index>0))
			throw InvalidArgument("bad plane index");
		format.fmt.pix.sizeimage=sizeimage;
	}
}

void v4l2_format_sizeimage(v4l2_format&format,uint32_t plane_index,uint32_t&sizeimage){
	if(v4l2_type_mplane_check(format.type)){
		if(unlikely(plane_index>=format.fmt.pix_mp.num_planes))
			throw InvalidArgument("bad plane index");
		sizeimage=format.fmt.pix_mp.plane_fmt[plane_index].sizeimage;
	}else{
		if(unlikely(plane_index>0))
			throw InvalidArgument("bad plane index");
		sizeimage=format.fmt.pix.sizeimage;
	}
}

void v4l2_format_pixel(v4l2_format&format,uint32_t&width,uint32_t&height,uint32_t&pixel_format){
	bool check=v4l2_type_mplane_check(format.type);
	width=check?format.fmt.pix_mp.width:format.fmt.pix.width;
	height=check?format.fmt.pix_mp.height:format.fmt.pix.height;
	pixel_format=check?format.fmt.pix_mp.pixelformat:format.fmt.pix.pixelformat;
}

void v4l2_buffer_setup_plane_length(v4l2_buffer&buffer,uint32_t plane_index,uint32_t length){
	if(v4l2_type_mplane_check(buffer.type)){
		if(unlikely(!buffer.m.planes||plane_index>=buffer.length))
			throw InvalidArgument("bad planes or plane index");
		buffer.m.planes[plane_index].length=length;
	}else{
		if(unlikely(plane_index>0))
			throw InvalidArgument("bad plane index");
		buffer.length=length;
	}
}

void v4l2_buffer_setup_plane_length_used(v4l2_buffer&buffer,uint32_t plane_index,uint32_t length){
	if(v4l2_type_mplane_check(buffer.type)){
		if(unlikely(!buffer.m.planes||plane_index>=buffer.length))
			throw InvalidArgument("bad planes or plane index");
		buffer.m.planes[plane_index].bytesused=length;
	}else{
		if(unlikely(plane_index>0))
			throw InvalidArgument("bad plane index");
		buffer.bytesused=length;
	}
}

void v4l2_buffer_plane_offset(v4l2_buffer&buffer,uint32_t plane_index,uint32_t&offset){
	if(v4l2_type_mplane_check(buffer.type)){
		if(unlikely(!buffer.m.planes||plane_index>=buffer.length))
			throw InvalidArgument("bad planes or plane index");
		offset=buffer.m.planes[plane_index].m.mem_offset;
	}else{
		if(unlikely(plane_index>0))
			throw InvalidArgument("bad plane index");
		offset=buffer.m.offset;
	}
}

void v4l2_buffer_plane_length(v4l2_buffer&buffer,uint32_t plane_index,uint32_t&length){
	if(v4l2_type_mplane_check(buffer.type)){
		if(unlikely(!buffer.m.planes||plane_index>=buffer.length))
			throw InvalidArgument("bad planes or plane index");
		length=buffer.m.planes[plane_index].length;
	}else{
		if(unlikely(plane_index>0))
			throw InvalidArgument("bad plane index");
		length=buffer.length;
	}
}

void v4l2_buffer_plane_length_used(v4l2_buffer&buffer,uint32_t plane_index,uint32_t&length){
	if(v4l2_type_mplane_check(buffer.type)){
		if(unlikely(!buffer.m.planes||plane_index>=buffer.length))
			throw InvalidArgument("bad planes or plane index");
		length=buffer.m.planes[plane_index].bytesused;
	}else{
		if(unlikely(plane_index>0))
			throw InvalidArgument("bad plane index");
		length=buffer.bytesused;
	}
}
