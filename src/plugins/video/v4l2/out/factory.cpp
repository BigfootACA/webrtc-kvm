/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_out.h"

V4L2Output::V4L2Output(webrtc_kvm*ctx){
	this->ctx=ctx;
}

Stream*V4L2OutputFactory::Create(webrtc_kvm*ctx){
	return new V4L2Output(ctx);
}

DECL_FACTORY(V4L2OutputFactory,v4l2_out)
