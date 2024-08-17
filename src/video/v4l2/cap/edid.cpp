/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_cap.h"
#include"lib/full_io.h"
#include"lib/readable.h"

void V4L2Capture::SetupEdid(){
	auto size=edid->GetLength();
	v4l2_edid e={
		.pad=0,
		.start_block=0,
		.blocks=(uint32_t)(size/256),
		.edid=edid->ToUCharPtr(),
	};
	log_info("v4l2 {} set edid size {}",GetID(),SizeStringFloat(size));
	xioctl(device_fd,VIDIOC_S_EDID,&e);
	try{
		v4l2_dv_timings dv{};
		xioctl(device_fd,VIDIOC_QUERY_DV_TIMINGS,&dv);
		xioctl(device_fd,VIDIOC_S_DV_TIMINGS,&dv);
	}catch(std::exception&exc){
		log_warn("v4l2 {} set dv timings failed: {}",GetID(),exc.what());
	}
}

void V4L2Capture::LoadEdidConfig(YAML::Node&cfg){
	if(auto v=cfg["edid"]){
		void*data=nullptr;
		auto path=v.as<std::string>();
		auto r=full_read_file_alloc(path.c_str(),&data);
		if(r<0)throw ErrnoException("read edid {} failed",path);
		edid=std::make_shared<Blob>(data,(size_t)r,true);
		log_info(
			"loaded edid file {} size {} for {}",
			path,SizeStringFloat((uint64_t)r),GetID()
		);
		if((edid->GetLength()%256)!=0)
			throw InvalidArgument("edid {} is not align to 256 bytes",path);
	}
}