/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef ASYNC_RUNNER_H
#define ASYNC_RUNNER_H
#include<map>
#include<mutex>
#include<queue>
#include<functional>
#include<semaphore.h>
#include<pthread.h>
#include"algo/uuid.h"
class AsyncRunner;
typedef std::function<void(AsyncRunner*runner)>AsyncFunction;
typedef std::function<void(AsyncRunner*runner,const UUID&id,AsyncFunction task)>TaskChangedHandler;
class AsyncRunner{
	public:
		~AsyncRunner();
		void RunPending();
		void StartThread();
		void StopThread();
		void AsyncWorkerMain();
		void RunLater(const AsyncFunction& cb);
		UUID AddOnTaskChanged(TaskChangedHandler handler);
		void RemoveOnTaskChanged(const UUID&id);
		static void DefaultRunLater(const AsyncFunction& cb);
		static void DefaultStartThread();
		static void DefaultStopThread();
		static AsyncRunner*GetDefault();
	private:
		pthread_t thread=0;
		sem_t sem;
		std::mutex lock;
		volatile bool running=false;
		std::queue<AsyncFunction>jobs;
		std::map<UUID,TaskChangedHandler>on_task_changed;
		static AsyncRunner*def;
};
#endif
