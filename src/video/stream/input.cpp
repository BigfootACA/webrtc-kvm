/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/log.h"
#include"abstract/async-runner.h"

void Stream::ProcessInput(StreamBuffer*buffer){
	if(unlikely(status!=STREAM_RUNNING))return;
	try{
		OnProcessInput(buffer);
		error/=2;
	}catch(std::exception&exc){
		log_warn("stream {} process input failed: {}",GetID(),exc.what());
		if(error>=ERROR_MAX){
			log_warn("too many error in {}, stop it...",GetID());
			AsyncRunner::DefaultRunLater([this](auto){StopStream();});
		}else error++;
	}
}
