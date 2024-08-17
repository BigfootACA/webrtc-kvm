/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"
#include"string/strings.h"

void V4L2Device::OnBindLink(std::shared_ptr<StreamLink>link,StreamLinkDirection dir){
	if(memory==0)switch(link->GetType()){
		case BUFFER_POINTER:memory=V4L2_MEMORY_MMAP;break;
		case BUFFER_DMABUF:memory=V4L2_MEMORY_DMABUF;break;
		default:;
	}
}
