/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/log.h"
#include"lib/exception.h"
#include"abstract/async-runner.h"

void Stream::ProcessInput(StreamBuffer*buffer){
	if(unlikely(status!=STREAM_RUNNING))return;
	try{
		OnProcessInput(buffer);
		error/=2;
	}catch(std::exception&exc){
		if(Exceptions::ErrnoExceptionImpl::IsErrno(exc,EAGAIN))return;
		log_warn("stream {} process input failed: {}",GetID(),exc.what());
		if(error>=ERROR_MAX){
			log_warn("too many error in {}, stop it...",GetID());
			AsyncRunner::DefaultRunLater([this](auto){StopStream();});
		}else error++;
	}
}

void Stream::SendToNext(StreamBuffer*buf){
	auto type=GetType();
	if(unlikely(type!=STREAM_SOURCE&&type!=STREAM_PIPE))
		throw InvalidArgument("cannot send stream when type is not source or pipe in {}",GetID());
	bool sent=false;
	for(auto output:outputs){
		auto sink=output->GetSink();
		if(unlikely(!sink))continue;
		sink->ProcessInput(buf);
		sent=true;
	}
	if(unlikely(!sent))throw RuntimeError("no any output to send stream");
}
