/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkrga.h"

class RockchipRasterGraphicAccelerationFactory:public StreamFactory{
	public:
		~RockchipRasterGraphicAccelerationFactory()override=default;
		inline RockchipRasterGraphicAccelerationFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()const final{return "rkrga";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

Stream*RockchipRasterGraphicAccelerationFactory::Create(webrtc_kvm*ctx){
	return new RockchipRasterGraphicAcceleration(ctx);
}

RockchipRasterGraphicAcceleration::RockchipRasterGraphicAcceleration(webrtc_kvm*ctx){
	this->ctx=ctx;
	if(ctx->video.fps!=0)fps=ctx->video.fps;
}

DECL_FACTORY(RockchipRasterGraphicAccelerationFactory,rkrga)
