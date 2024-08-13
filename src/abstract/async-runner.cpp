/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<mutex>
#include<utility>
#include<unistd.h>
#include<pthread.h>
#include"async-runner.h"
#include"lib/log.h"
#include"lib/exception.h"
#include"defines.h"

AsyncRunner*AsyncRunner::def=nullptr;

void AsyncRunner::RunPending(){
	while(!jobs.empty()){
		lock.lock();
		auto callback=jobs.front();
		jobs.pop();
		lock.unlock();
		try{
			callback(this);
		}catch(std::exception&exc){
			log_warn("unknown exception while run async task: {}",exc.what());
		}
	}
}

static void*StartAsyncWorker(void*d){
	auto a=(AsyncRunner*)d;
	if(likely(a))a->AsyncWorkerMain();
	return nullptr;
}

void AsyncRunner::StartThread(){
	if(unlikely(running))return;
	sem_init(&sem,0,0);
	auto r=pthread_create(&thread,nullptr,StartAsyncWorker,this);
	if(unlikely(r!=0))throw RuntimeError("create async worker thread failed");
	pthread_setname_np(thread,"Async Worker");
	log_info("async worker started");
	while(!running)usleep(100000);
}

void AsyncRunner::StopThread(){
	if(unlikely(!running))return;
	running=false;
	sem_post(&sem);
	pthread_join(thread,nullptr);
	sem_destroy(&sem);
}

void AsyncRunner::AsyncWorkerMain(){
	running=true;
	while(likely(running)){
		RunPending();
		sem_wait(&sem);
	}
}

AsyncRunner::~AsyncRunner(){
	if(def==this)def=nullptr;
}

void AsyncRunner::RunLater(const AsyncFunction&cb){
	std::lock_guard<std::mutex>lk(lock);
	for(const auto&kv:on_task_changed)
		kv.second(this,kv.first,cb);
	jobs.push(cb);
	if(running)sem_post(&sem);
}

void AsyncRunner::DefaultRunLater(const AsyncFunction&cb){
	auto d=GetDefault();
	if(unlikely(!d))throw RuntimeError("no default runner");
	d->RunLater(cb);
}

void AsyncRunner::DefaultStartThread(){
	auto d=GetDefault();
	if(unlikely(!d))throw RuntimeError("no default runner");
	d->StartThread();
}

void AsyncRunner::DefaultStopThread(){
	auto d=GetDefault();
	if(unlikely(!d))throw RuntimeError("no default runner");
	d->StopThread();
}

AsyncRunner*AsyncRunner::GetDefault(){
	if(!def)def=new AsyncRunner;
	return def;
}

UUID AsyncRunner::AddOnTaskChanged(TaskChangedHandler handler){
	UUID id;
	id.Generate();
	on_task_changed[id]=std::move(handler);
	return id;
}

void AsyncRunner::RemoveOnTaskChanged(const UUID&id){
	auto i=on_task_changed.find(id);
	if(i==on_task_changed.end())return;
	on_task_changed.erase(i);
}
