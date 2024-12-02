/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"posix.h"

class PosixInputStreamFactory:public StreamFactory{
	public:
		~PosixInputStreamFactory()override=default;
		inline PosixInputStreamFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()const final{return "posix-in";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

Stream*PosixInputStreamFactory::Create(webrtc_kvm*ctx){
	return new PosixInputStream(ctx);
}

PosixInputStream::PosixInputStream(webrtc_kvm*ctx){
	this->ctx=ctx;
}

class PosixOutputStreamFactory:public StreamFactory{
	public:
		~PosixOutputStreamFactory()override=default;
		inline PosixOutputStreamFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()const final{return "posix-out";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

Stream*PosixOutputStreamFactory::Create(webrtc_kvm*ctx){
	return new PosixOutputStream(ctx);
}

PosixOutputStream::PosixOutputStream(webrtc_kvm*ctx){
	this->ctx=ctx;
}

DECL_FACTORY(PosixInputStreamFactory,posix_in)
DECL_FACTORY(PosixOutputStreamFactory,posix_out)
