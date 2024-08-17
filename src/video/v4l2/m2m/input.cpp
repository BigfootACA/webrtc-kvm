/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"

void V4L2MemoryToMemory::OnProcessInput(StreamBuffer*input){
	static bool warned=false;
	if(unlikely(out->buffers.size()!=cap->buffers.size()))
		throw RuntimeError("output buffers and capture buffers mismatch");
	auto buffer=out->FindAvailableBuffer();
	if(!buffer){
		out->DequeueBuffer(&buffer);
		if(!buffer){
			if(!warned)log_warn("stream {} no available buffer",GetID());
			warned=true;
			return;
		}
		if(buffer->status==STATUS_DEQUEUED)buffer->status=STATUS_IDLE;
	}
	out->QueueInput(input,buffer);
	cap->QueueBuffer(cap->buffers[buffer->idx]);
	if(unlikely(poll_id.IsNull()))EnablePoll();
	warned=false;
}
