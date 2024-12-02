/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_cap.h"

void V4L2Capture::OnBindLink(std::shared_ptr<StreamLink>link,StreamLinkDirection dir){
	if(dir==LINK_DIR_OUT&&memory==0)switch(link->GetType()){
		case BUFFER_POINTER:
		case BUFFER_DMABUF:
			memory=V4L2_MEMORY_MMAP;
		break;
		default:;
	}
	V4L2Device::OnBindLink(link,dir);
}
