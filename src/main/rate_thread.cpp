/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc_kvm.h"
#include"lib/exception.h"
#include"lib/readable.h"
#include"lib/log.h"
#include"video/stream.h"

static void*RateThreadWorker(void*data){
	//char buff[64];
	auto kvm=(webrtc_kvm*)data;
	//bool print=isatty(STDOUT_FILENO);
	while(likely(kvm->running)){
		//pthread_mutex_lock(&kvm->lock);
		//kvm->video.rate_frames=kvm->video.frames-kvm->video.last_frames;
		//kvm->video.rate_outbound=kvm->video.outbound-kvm->video.last_outbound;
		//kvm->video.last_frames=kvm->video.frames;
		//kvm->video.last_outbound=kvm->video.outbound;
		//if(kvm->video.stream_refcnt>0&&print){
		//	printf(
		//		"\r\33[2K\rRunning encoder %" PRIu64 " FPS, %s/s\r",
		//		kvm->video.rate_frames,format_size_float(buff,kvm->video.rate_outbound)
		//	);
		//	fflush(stdout);
		//}
		//if(kvm->video.capture.error+kvm->video.source.error+kvm->video.output.error>ERROR_MAX){
		//	log_err("too many error, stop encoder");
		//	//stream_stop(kvm);
		//}
		//pthread_mutex_unlock(&kvm->lock);
		sleep(1);
		kvm->dog+=1;
		if(kvm->dog>=10)
			throw RuntimeError("watchdog timeout, main thread maybe dead");
	}
	return NULL;
}

void InitializeRateThread(webrtc_kvm*ctx){
	if(pthread_create(&ctx->rate_tid,nullptr,RateThreadWorker,ctx)!=0)
		throw ErrnoException("create rate thread failed");
}
