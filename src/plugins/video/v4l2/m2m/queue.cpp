/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"

void V4L2MemoryToMemory::OnProcessNeedQueue(){
	bool queued=false;
	for(auto buf:out->buffers)switch(buf->status){
		case STATUS_PREPARED:
			out->QueueBuffer(buf);
			cap->QueueBuffer(cap->buffers[buf->idx]);
			queued=true;
		break;
		default:;
	}
	if(!queued)DisablePoll();
}
