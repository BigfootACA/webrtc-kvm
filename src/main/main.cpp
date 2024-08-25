/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<err.h>
#include<getopt.h>
#include<csignal>
#include<sys/sysmacros.h>
#include"webrtc_kvm.h"
#include"lib/exception.h"
#include"abstract/async-runner.h"
#include"abstract/loop.h"
#include"lib/log.h"

struct webrtc_kvm ctx{};

static const char*opts="f:Th";

static struct option lopts[]={
	{"file",    required_argument, nullptr, 'f'},
	{"test",    no_argument,       nullptr, 'T'},
	{"help",    no_argument,       nullptr, 'h'},
	{nullptr,0,nullptr,0}
};

static int usage(int err){
	fprintf(
		err==0?stdout:stderr,
		"Usage: webrtc-kvm [OPTIONS]...\n"
		"Options:\n"
		"\t-T, --test                Run unit tests\n"
		"\t-f, --file <CONFIG>       YAML configuration file\n"
	);
	return err;
}

static void HandInterrupt(int sig __attribute__((unused))){
	printf("\r\33[2K\rExiting...\r\n");
	SetRunning(&ctx,false);
}

int main(int argc,char**argv){
	int o;
	bool test=false;
	try{
		bool success=true;
		ConfigPopulateDefault(&ctx);
		while((o=getopt_long(argc,argv,opts,lopts,nullptr))>=0)switch(o){
			case 'f':if(!ConfigLoadFile(&ctx,optarg))success=false;break;
			case 'T':test=true;break;
			case 'h':return usage(0);
			default:return usage(1);
		}
		if(!success)throw RuntimeError("config load failed");
		if(optind!=argc)return usage(1);
	}catch(std::exception&exc){
		log_fatal("parse options failed: {}",exc.what());
		return 1;
	}
	if(test)return RunUnitTest();
	try{
		ctx.running=true;
		InitializeMain(&ctx);
		InitializeInput(&ctx);
		InitializeGadget(&ctx);
		InitializeHTTP(&ctx);
		InitializeRateThread(&ctx);
	}catch(std::exception&exc){
		log_fatal("initialize failed: {}",exc.what());
		return 1;
	}
	signal(SIGINT,HandInterrupt);
	signal(SIGHUP,HandInterrupt);
	signal(SIGTERM,HandInterrupt);
	signal(SIGPIPE,SIG_IGN);
	auto runner=AsyncRunner::GetDefault();
	while(likely(ctx.running)){
		ctx.loop->RunOnceLoop(1000);
		runner->RunPending();
		ctx.dog=0;
	}
	return 0;
}
