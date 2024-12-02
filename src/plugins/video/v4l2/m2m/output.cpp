/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"

void V4L2MemoryToMemory::OnProcessOutput(){
	V4L2StreamBuffer*cap_buf=nullptr;
	bool have_cap_queued=false;
	for(auto buf:cap->buffers)if(buf->status==STATUS_QUEUED)have_cap_queued=true;
	if(unlikely(!have_cap_queued)){
		log_warn("no any buffer to dequeue in {}",GetID());
		DisablePoll();
		return;
	}
	if(have_cap_queued)cap->DequeueBuffer(&cap_buf);
	if(cap_buf)cap->ProcessNextInput(cap_buf);
}
