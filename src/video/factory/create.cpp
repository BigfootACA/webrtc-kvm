/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/exception.h"

Stream*StreamFactory::CreateFromConfig(webrtc_kvm*ctx,YAML::Node&cfg){
	auto stream=Create(ctx);
	if(unlikely(!stream))throw RuntimeError("create stream failed");
	stream->LoadConfig(cfg);
	return stream;
}

Stream*StreamFactory::CreateStreamByDriverName(webrtc_kvm*ctx,const std::string&driver){
	StreamFactory*factory=GetFactoryByDriverName(driver);
	if(unlikely(!factory))throw RuntimeError("driver {} not found",driver);
	return factory->Create(ctx);
}

Stream*StreamFactory::CreateStreamFromConfig(webrtc_kvm*ctx,YAML::Node&cfg){
	auto drv=cfg["driver"];
	if(unlikely(!drv.IsDefined()))throw RuntimeError("driver not set for stream");
	auto driver=drv.as<std::string>();
	StreamFactory*factory=GetFactoryByDriverName(driver);
	if(unlikely(!factory))throw RuntimeError("driver {} not found",driver);
	return factory->CreateFromConfig(ctx,cfg);
}
