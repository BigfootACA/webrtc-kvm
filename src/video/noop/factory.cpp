/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"noop.h"

class NoopSinkFactory:public StreamFactory{
	public:
		~NoopSinkFactory()override=default;
		inline NoopSinkFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()const final{return "noop-out";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

cdecl_attr_used NoopSinkFactory noop_out_factory;

Stream*NoopSinkFactory::Create(webrtc_kvm*ctx){
	return new NoopSink(ctx);
}

NoopSink::NoopSink(webrtc_kvm*ctx){
	this->ctx=ctx;
}

class NoopPipeFactory:public StreamFactory{
	public:
		~NoopPipeFactory()override=default;
		inline NoopPipeFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()const final{return "noop-pipe";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

cdecl_attr_used NoopPipeFactory noop_pipe_factory;

Stream*NoopPipeFactory::Create(webrtc_kvm*ctx){
	return new NoopPipe(ctx);
}

NoopPipe::NoopPipe(webrtc_kvm*ctx){
	this->ctx=ctx;
}
