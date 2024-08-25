/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"

class RockchipMediaProcessPlatformFactory:public StreamFactory{
	public:
		~RockchipMediaProcessPlatformFactory()override=default;
		inline RockchipMediaProcessPlatformFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()const final{return "rkmpp";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

cdecl_attr_used RockchipMediaProcessPlatformFactory rkmpp_factory;

Stream*RockchipMediaProcessPlatformFactory::Create(webrtc_kvm*ctx){
	return new RockchipMediaProcessPlatform(ctx);
}

RockchipMediaProcessPlatform::RockchipMediaProcessPlatform(webrtc_kvm*ctx){
	this->ctx=ctx;
	if(ctx->video.fps!=0)fps=ctx->video.fps;
}
