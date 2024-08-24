/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<utility>
#include"lib/log.h"
#include"lib/exception.h"
#include"algo/uuid.h"
#include"webrtc_kvm.h"
#include"async-runner.h"
#include"loop.h"

EventLoop::~EventLoop(){
	std::unique_lock<std::mutex>lk(lock);
	if(epoll_fd>=0)close(epoll_fd);
	epoll_fd=-1;
}

UUID EventLoop::LockedAddHandler(int fd,uint32_t events,EventHandler handler){
	auto ev=std::make_shared<EventHandlerData>();
	ev->fd=fd,ev->events=fd,ev->handler=std::move(handler);
	ev->error=0,ev->max_error=10;
	ev->uuid.Generate();
	ev->epoll.events=events;
	ev->epoll.data.ptr=ev.get();
	if(handlers.find(ev->uuid)!=handlers.end())
		throw RuntimeError("handler already exists");
	int ret=epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&ev->epoll);
	if(ret<0)throw ErrnoException("epoll_ctl add failed");
	handlers[ev->uuid]=ev;
	CallHandler(ev.get(),TYPE_INIT);
	return ev->uuid;
}

void EventLoop::LockedRemoveHandler(const UUID&id){
	auto idx=handlers.find(id);
	if(idx==handlers.end())return;
	auto ev=idx->second;
	CallHandler(ev.get(),TYPE_REMOVE);
	int ret=epoll_ctl(epoll_fd,EPOLL_CTL_DEL,ev->fd,&ev->epoll);
	if(ret<0)throw ErrnoException("epoll_ctl del failed");
	handlers.erase(idx);
}

int EventLoop::CreateEpoll(){
	int r=epoll_create1(0);
	if(r<0)throw ErrnoException("epoll create failed");
	return r;
}


void EventLoop::CallHandler(EventHandlerData*data,EventCallbackType type,epoll_event*ev){
	EventHandlerContext ctx={.ev=data,.type=type,.epoll=ev,.event=ev?ev->events:0};
	data->handler(&ctx);
}

void EventLoop::LockedProcessEvent(epoll_event*ev){
	auto data=(EventHandlerData*)ev->data.ptr;
	try{
		CallHandler(data,TYPE_EVENTS,ev);
		data->error/=2;
	}catch(std::exception&exc){
		if(Exceptions::ErrnoExceptionImpl::IsErrno(exc,EAGAIN))return;
		log_warn("error while process fd {}: {}",data->fd,exc.what());
		auto type=TYPE_ERROR;
		data->error++;
		if(data->error>=data->max_error){
			log_err("too many exception in FD {}, auto remove it",data->fd);;
			type=TYPE_FATAL;
		}
		try{CallHandler(data,type);}catch(...){}
		if(type==TYPE_FATAL)try{LockedRemoveHandler(data->uuid);}catch(...){}
	}
}

void EventLoop::LockedProcessEvents(epoll_event*evs,int count){
	for(int event=0;event<count;event++)
		LockedProcessEvent(&evs[event]);
}

void EventLoop::RunOnceLoop(int timeout){
	int ret;
	epoll_event events[8];
	if((ret=epoll_wait(epoll_fd,events,ARRAY_SIZE(events),timeout))<0){
		if(errno==EINTR||errno==EAGAIN)return;
		throw ErrnoException("epoll failed");
	}
	ProcessEvents(events,ret);
}
