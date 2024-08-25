/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"

void V4L2MemoryToMemory::OnBindLink(
	std::shared_ptr<StreamLink>link,
	StreamLinkDirection dir
){
	switch(dir){
		case LINK_DIR_IN:
			out->BindLink(link,dir);
			out->input=link;
		break;
		case LINK_DIR_OUT:
			cap->BindLink(link,dir);
			cap->outputs.push_back(link);
		break;
		default:;
	}
}
