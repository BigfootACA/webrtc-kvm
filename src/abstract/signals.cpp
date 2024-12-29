/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<map>
#include<queue>
#include<utility>
#include<unistd.h>
#include"lib/log.h"
#include"lib/exception.h"
#include"signals.h"
#include"async-runner.h"

#define SIG_MAX 64
struct SignalHandlers{
	time_t interval=0;
	std::map<std::string,Signal*>hands;
};
struct SignalAsyncJob{
	int signal;
	siginfo_t info;
};
struct SignalData{
	bool async=false;
	SignalHandlers signals[SIG_MAX+1];
	std::queue<SignalAsyncJob*>jobs;
}job_data;

static void SignalJobRunner(SignalAsyncJob*job){
	SignalContext ctx{};
	std::vector<Signal*>removes;
	auto&m=job_data.signals[job->signal].hands;
	if(m.empty())return;
	ctx.info=&job->info;
	ctx.signal=job->signal;
	for(auto l:m){
		auto cb=l.second;
		ctx.listener=cb;
		try{
			if(!cb->Run(&ctx))removes.push_back(cb);
		}catch(std::exception&exc){
			log_warn(
				"unknown exception while process signal {}: {}",
				job->signal,exc.what()
			);
		}
	}
	for(auto cb:removes)delete cb;
}

static void SignalHandler(int sig,siginfo_t*info,void*d cdecl_attr_unused){
	auto job=new SignalAsyncJob;
	job->signal=sig;
	memcpy(&job->info,info,sizeof(job->info));
	job_data.jobs.push(job);
	if(!job_data.async)SignalJobRunner(job);
	else AsyncRunner::GetDefault()->RunLater([job](auto){
		SignalJobRunner(job);
	});
}

static void InstallListener(int sig){
	struct sigaction sa{};
        sigemptyset(&sa.sa_mask);
        sa.sa_sigaction=&SignalHandler;
        sa.sa_flags=SA_SIGINFO|SA_ONSTACK;
        sigaction(sig,&sa,nullptr);
}

static void UninstallListener(int sig){
	signal(sig,SIG_DFL);
}

void Signal::Unsubscribe(){
	auto&m=job_data.signals[signal].hands;
	if(!m.contains(name))return;
	m.erase(name);
	if(m.empty())UninstallListener(signal);
}

SignalTimer::~SignalTimer(){
	Unsubscribe();
	auto&s=job_data.signals[signal];
	if(s.hands.empty()){
		alarm(0);
		s.interval=0;
	}
}

static std::string GenName(std::string ns){
	auto id=UUID::Random();
	return std::format("{}-{}",ns,id.ToString());
}

Signal::Signal(int sig,const std::string&name,SignalCallback callback,void*data){
	if(sig<=0||sig>SIG_MAX)throw InvalidArgument("bad signal");
	auto&m=job_data.signals[sig].hands;
	if(m.contains(name))throw InvalidArgument("duplicate name");
	this->name=name,this->signal=sig;
	this->callback=std::move(callback),this->data=data;
	if(m.size()==0)InstallListener(sig);
	m[name]=this;
}

SignalTimer::SignalTimer(uint32_t seconds,const std::string&name,SignalCallback callback,void*data)
	:Signal(SIGALRM,name,std::move(callback),data){
	SetInterval(seconds);
}

void SignalTimer::SetInterval(time_t val){
	if(val<=0)throw InvalidArgument("bad interval");
	auto&s=job_data.signals[SIGALRM];
	interval=val;
	if(s.interval==0||s.interval>val){
		s.interval=val;
		alarm(val);
	}
}

bool SignalTimer::Trigger(SignalContext*ctx){
	time_t now=time(nullptr);
	if(GetLastTime()>now)return true;
	if(times<=0)return false;
	times--;
	if(!callback(ctx))return false;
	return times>0;
}

Signal::Signal(int sig,SignalCallback callback,void*data)
	:Signal(sig,GenName("signal"),std::move(callback),data){}
SignalTimer::SignalTimer(uint32_t seconds,SignalCallback callback,void*data)
	:SignalTimer(seconds,GenName("timer"),std::move(callback),data){}

void Signal::SetAsync(bool async){
	job_data.async=async;
}
