/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_cap.h"
#include"lib/strings.h"
#include"abstract/resource_release.h"
#include"../media/entity.h"
#include"../media/device.h"
#include"../media/topology.h"
#include"../media/entity_desc.h"
#include<linux/v4l2-subdev.h>

static std::vector<std::shared_ptr<MediaEntity>>GetSubdevs(
	const std::shared_ptr<MediaTopology>&topo,
	dev_t dev
){
	std::vector<std::shared_ptr<MediaEntity>>subdevs;
	auto self=topo->FindEntityByDevice(dev);
	if(!self)throw RuntimeError("cannot get self in media topology");
	for(const auto&ent:self->FindSourceEntitiesRecursive()){
		auto desc=ent->GetEntityDesc();
		if(!desc||!desc->HaveType(MEDIA_ENT_T_V4L2_SUBDEV))continue;
		log_info("found {}",ent->ToString());
		subdevs.push_back(ent);
	}
	return subdevs;
}

static void LoadFromUpstream(
	const std::vector<std::shared_ptr<MediaEntity>>&subdevs,
	dev_t dev,uint32_t&width,uint32_t&height,uint32_t&mbus_code
){
	for(const auto&subdev:subdevs){
		if(!subdev->FindSourceEntities().empty())continue;
		log_info("use upstream device {}",subdev->ToString());
		bool found_dev=false;
		std::exception_ptr p=nullptr;
		for(auto device:subdev->FindDevices())try{
			v4l2_subdev_format fmt{};
			FDRelease r(DeviceOpen(S_IFCHR,device));
			fmt.which=V4L2_SUBDEV_FORMAT_ACTIVE;
			xioctl(r.Get(),VIDIOC_SUBDEV_G_FMT,&fmt);
			log_dbg("upstream size: {}x{}",fmt.format.width,fmt.format.height);
			log_dbg("upstream mbus code: 0x{:x}",fmt.format.code);
			if(width==0&&fmt.format.width!=0)width=fmt.format.width;
			if(height==0&&fmt.format.height!=0)height=fmt.format.height;
			if(mbus_code==0&&fmt.format.code!=0)mbus_code=fmt.format.code;
			found_dev=true;
			break;
		}catch(...){
			p=std::current_exception();
		}
		if(!found_dev){
			if(p)std::rethrow_exception(p);
			throw RuntimeError("failed to get subdev format from devices");
		}
	}
}

static void SetupSubdevs(
	const std::vector<std::shared_ptr<MediaEntity>>&subdevs,
	dev_t dev,uint32_t&width,uint32_t&height,uint32_t&mbus_code
){
	for(const auto&subdev:subdevs)for(auto device:subdev->FindDevices())try{
		v4l2_subdev_format fmt{};
		FDRelease r(DeviceOpen(S_IFCHR,device));
		fmt.which=V4L2_SUBDEV_FORMAT_ACTIVE;
		xioctl(r.Get(),VIDIOC_SUBDEV_G_FMT,&fmt);
		fmt.format.width=width;
		fmt.format.height=height;
		fmt.format.code=mbus_code;
		xioctl(r.Get(),VIDIOC_SUBDEV_S_FMT,&fmt);
		log_dbg(
			"initialized subdev {} ({})",
			subdev->GetName(),DeviceToString(device)
		);
	}catch(std::exception&exc){
		log_warn(
			"setup subdev {} ({}) failed: {}",
			subdev->GetName(),DeviceToString(device),exc.what()
		);
	}
}

void V4L2Capture::SetupSubdev(){
	dev_t dev;
	std::shared_ptr<MediaDevice>media;
	try{
		dev=DeviceFromFD(device_fd,S_IFCHR);
		if(!(media=MediaDevice::FromVideoDevice(dev)))
			throw InvalidArgument("failed to create media");
		log_info("found {}",media->ToString());
	}catch(std::exception&exc){
		log_warn("create media failed: {}",exc.what());
		return;
	}
	std::shared_ptr<MediaTopology>topo;
	std::vector<std::shared_ptr<MediaEntity>>subdevs;
	try{
		topo=media->GetTopology();
		if(!topo)throw RuntimeError("failed to get media topology");
		subdevs=GetSubdevs(topo,dev);
	}catch(std::exception&exc){
		log_warn("cannot get subdevs: {}",exc.what());
		return;
	}
	try{
		if(width==0||height==0||mbus_code==0)
			LoadFromUpstream(subdevs,dev,width,height,mbus_code);
		SetupSubdevs(subdevs,dev,width,height,mbus_code);
	}catch(std::exception&exc){
		log_warn("cannot setup subdevs: {}",exc.what());
		return;
	}
}
