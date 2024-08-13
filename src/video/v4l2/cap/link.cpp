/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_cap.h"


void V4L2Capture::OnBindOutput(std::shared_ptr<StreamLink>link){
	if(memory==0)switch(link->type){
		case BUFFER_POINTER:
		case BUFFER_DMABUF:
			memory=V4L2_MEMORY_MMAP;
		break;
		default:;
	}
	V4L2Device::OnBindOutput(link);
}
