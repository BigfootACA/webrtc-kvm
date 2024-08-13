/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<sys/eventfd.h>
#include"webrtc_kvm.h"
#include"lib/exception.h"
#include"abstract/loop.h"
#include"abstract/async-runner.h"

void SetRunning(struct webrtc_kvm*ctx,bool running){
	eventfd_t value=1;
	ctx->running=running;
	eventfd_write(ctx->event_fd,value);
}

void InitializeMain(webrtc_kvm*ctx){
	int ret;
	try{
		auto runner=AsyncRunner::GetDefault();
		ctx->loop=new EventLoop();
		if((ret=eventfd(0,EFD_SEMAPHORE|EFD_NONBLOCK|EFD_CLOEXEC))<0)
			throw ErrnoException("create eventfd failed");
		ctx->event_fd=ret;
		ctx->loop->AddHandler(ctx->event_fd,EPOLLIN,[](auto e){
			if(e->type==TYPE_EVENTS){
				eventfd_t val;
				eventfd_read(e->ev->fd,&val);
			}
		});
		runner->AddOnTaskChanged([ctx](auto,auto,auto){
			eventfd_t value=1;
			eventfd_write(ctx->event_fd,value);
		});
	}catch(...){
		if(ctx->event_fd>0)close(ctx->event_fd);
		ctx->event_fd=-1;
		throw;
	}
}
