/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"swscale.h"

class SwscalePipeFactory:public StreamFactory{
	public:
		~SwscalePipeFactory()override=default;
		inline SwscalePipeFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()const final{return "swscale";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

cdecl_attr_used SwscalePipeFactory swscale_out_factory;

Stream*SwscalePipeFactory::Create(webrtc_kvm*ctx){
	return new SwscalePipe(ctx);
}

SwscalePipe::SwscalePipe(webrtc_kvm*ctx){
	this->ctx=ctx;
	this->fmt_in=AV_PIX_FMT_NONE;
	this->fmt_out=AV_PIX_FMT_NONE;
}
