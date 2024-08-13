/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef SIGNALS_H
#define SIGNALS_H
#include<format>
#include<string>
#include<cstdint>
#include<functional>
#include <utility>
#include<signal.h>
#include"uuid.h"
#include"defines.h"
class Signal;
struct SignalContext;
typedef std::function<bool(SignalContext*ctx)>SignalCallback;
struct SignalContext{
	int signal;
	siginfo_t*info;
	Signal*listener;
};
class Signal{
	public:
		Signal(int sig,SignalCallback callback,void*data);
		Signal(int sig,const std::string&name,SignalCallback callback,void*data);
		inline Signal(int sig,SignalCallback callback)
			:Signal(sig,std::move(callback),nullptr){}
		inline Signal(int sig,const std::string&name,SignalCallback callback)
			:Signal(sig,name,std::move(callback),nullptr){}
		virtual inline ~Signal(){Unsubscribe();}
		[[nodiscard]] inline void*GetData(){return data;}
		[[nodiscard]] inline int GetSignal()const{return signal;}
		[[nodiscard]] inline std::string GetName()const{return name;}
		[[nodiscard]] inline SignalCallback GetCallback()const{return callback;}
		virtual inline bool Run(SignalContext*ctx){return callback(ctx);}
		inline bool operator()(SignalContext*ctx){return Run(ctx);}
		void Unsubscribe();
		static void SetAsync(bool async);
	protected:
		int signal=0;
		std::string name;
		SignalCallback callback;
		void*data=nullptr;
};
class SignalTimer:public Signal{
	public:
		~SignalTimer()override;
		SignalTimer(uint32_t seconds,SignalCallback callback,void*data);
		SignalTimer(uint32_t seconds,const std::string&name,SignalCallback callback,void*data);
		inline SignalTimer(uint32_t seconds,SignalCallback callback):
			SignalTimer(seconds,std::move(callback),nullptr){}
		inline SignalTimer(uint32_t seconds,const std::string&name,SignalCallback callback):
			SignalTimer(seconds,name,std::move(callback),nullptr){}
		void SetInterval(time_t val);
		virtual bool Trigger(SignalContext*ctx);
		inline bool Run(SignalContext*ctx)override{return Trigger(ctx);}
		[[nodiscard]] inline time_t GetInterval(){return interval;}
		[[nodiscard]] inline time_t GetNextTime(){return last_time+interval;}
		[[nodiscard]] inline time_t GetLastTime(){return last_time;}
		[[nodiscard]] inline size_t GetTimes(){return times;}
		inline void SetTimes(size_t val){times=val;}
	protected:
		time_t interval=0;
		time_t last_time=0;
		size_t times=SIZE_MAX;
};
#endif
