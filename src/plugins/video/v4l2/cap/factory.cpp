/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_cap.h"

Stream*V4L2CaptureFactory::Create(webrtc_kvm*ctx){
	return new V4L2Capture(ctx);
}

DECL_FACTORY(V4L2CaptureFactory,v4l2_cap)
