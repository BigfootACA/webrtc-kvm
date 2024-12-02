/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"

void RockchipMediaProcessPlatform::OnStartStream(){

}

void RockchipMediaProcessPlatform::OnStopStream(){
	for(auto dmabuf:dmabufs)
		mpp_buffer_put(dmabuf.second->buff);
	dmabufs.clear();
}
