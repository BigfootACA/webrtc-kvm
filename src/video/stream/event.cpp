/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/log.h"
#include"abstract/async-runner.h"

void Stream::ProcessEvent(EventHandlerContext*ev){
	switch(ev->type){
		case TYPE_EVENTS:
			if(have_bit(ev->event,EPOLLERR))
				OnProcessNeedQueue();
			if(have_bit(ev->event,EPOLLIN))
				OnProcessOutput();
		break;
		case TYPE_FATAL:
			log_warn("stream got error, stop stream {}...",GetID());
			AsyncRunner::DefaultRunLater([this](auto){StopStream();});
		break;
		case TYPE_INIT:ev->ev->max_error=ERROR_MAX;break;
		case TYPE_REMOVE:poll_id.Clear();break;
		default:;
	}
}
