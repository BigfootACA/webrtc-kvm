/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"
#include"lib/strings.h"
#include"abstract/resource_release.h"

void V4L2Device::FindMatchDevice(){
	uint32_t cnt=0,cap=0;
	if(!path.empty())return;
	if(!match.card.empty()){log_dbg("match {} using card {}",GetID(),match.card);cnt++;}
	if(!match.driver.empty()){log_dbg("match {} using driver {}",GetID(),match.driver);cnt++;}
	if(cnt==0)throw InvalidArgument("no device path set and no any match");
	auto devices=DevicesFromSubsystem("video4linux");
	if(devices.empty())throw InvalidArgument("no any video4linux devices for match");
	for(auto dev:devices)try{
		auto fd=DeviceOpen(S_IFCHR,dev);
		FDRelease release(fd);
		std::string driver,card;
		v4l2_capabilities_probe(fd,cap,driver,card);
		if(!match.card.empty()&&card!=match.card)continue;
		if(!match.driver.empty()&&driver!=match.driver)continue;
		if(DetectType(cap)==0)continue;
		path=PathFromDevice(S_IFCHR,dev);
		log_info("found match device {} for {}",path,GetID());
		return;
	}catch(std::exception&exc){
		log_dbg(
			"error while match {} failed: {}",
			DeviceToString(dev),exc.what()
		);
	}
	throw RuntimeError("no match device found for {}",GetID());
}
