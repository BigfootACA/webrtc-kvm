/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_out.h"

void V4L2Output::OnProcessInput(StreamBuffer*input){
	if(auto buf=FindAvailableBuffer())
		QueueInput(input,buf);
}

V4L2StreamBuffer*V4L2Output::FindAvailableBuffer(){
	for(auto buffer:buffers)
		if(buffer->status!=STATUS_QUEUED)
			return buffer;
	return nullptr;
}
