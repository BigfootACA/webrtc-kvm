/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef LOOP_H
#define LOOP_H
#include<map>
#include<mutex>
#include<memory>
#include<functional>
#include <utility>
#include<sys/epoll.h>
#include"algo/uuid.h"

struct EventHandlerData;
struct EventHandlerContext;
typedef std::function<void(EventHandlerContext*ev)>EventHandler;

struct EventHandlerData{
	UUID uuid;
	int fd;
	uint32_t events;
	epoll_event epoll;
	EventHandler handler;
	uint32_t error;
	uint32_t max_error;
};

enum EventCallbackType{
	TYPE_INIT,
	TYPE_EVENTS,
	TYPE_ERROR,
	TYPE_FATAL,
	TYPE_REMOVE,
};

struct EventHandlerContext{
	EventHandlerData*ev;
	EventCallbackType type;
	epoll_event*epoll;
	uint32_t event;
};

class EventLoop{
	public:
		inline explicit EventLoop(int fd,bool auto_close):epoll_fd(fd),auto_close(auto_close){}
		inline explicit EventLoop():EventLoop(CreateEpoll(),true){}
		~EventLoop();
		inline UUID AddHandler(int fd,uint32_t events,EventHandler handler){
			std::unique_lock<std::mutex>lk(lock);
			return LockedAddHandler(fd,events,std::move(handler));
		}
		inline void RemoveHandler(const UUID&id){
			std::unique_lock<std::mutex>lk(lock);
			LockedRemoveHandler(id);
		}
		inline void ProcessEvent(epoll_event*ev){
			std::unique_lock<std::mutex>lk(lock);
			LockedProcessEvent(ev);
		}
		inline void ProcessEvents(epoll_event*evs,int count){
			std::unique_lock<std::mutex>lk(lock);
			LockedProcessEvents(evs,count);
		}
		void RunOnceLoop(int timeout=-1);
		static int CreateEpoll();
	private:
		UUID LockedAddHandler(int fd,uint32_t events,EventHandler handler);
		void LockedRemoveHandler(const UUID&id);
		void CallHandler(EventHandlerData*data,EventCallbackType type,epoll_event*ev=nullptr);
		void LockedProcessEvent(epoll_event*ev);
		void LockedProcessEvents(epoll_event*evs,int count);
		int epoll_fd;
		bool auto_close;
		std::mutex lock;
		std::map<UUID,std::shared_ptr<EventHandlerData>>handlers;
};

#endif
