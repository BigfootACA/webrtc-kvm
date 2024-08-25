/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/log.h"

void Stream::StopStream(bool all,bool deinit){
	if(!outputs.empty())for(auto output:outputs){
		auto sink=GetOutputStream();
		if(!sink||sink->GetStatus()!=STREAM_RUNNING)continue;
		log_dbg(
			"stop dependency sink stream {} requested by {}",
			sink->GetID(),GetID()
		);
		sink->StopStream();
		if(unlikely(sink->GetStatus()==STREAM_RUNNING)){
			log_warn(
				"sink stream {} depend on source stream {}, but cannot stop it",
				sink->GetID(),GetID()
			);
			return;
		}
	}
	if(status==STREAM_RUNNING)try{
		log_dbg("stopping stream {}",GetID());
		OnStopStream();
		error=0;
		status=STREAM_STOPPED;
		log_dbg("stopped stream {}",GetID());
	}catch(std::exception&exc){
		log_warn("failed to stop stream {}: {}",GetID(),exc.what());
		return;
	}
	if(deinit)if(status==STREAM_STOPPED)try{
		log_dbg("uninitializing stream {}",GetID());
		OnDeinitialize();
		status=STREAM_UNINITIALIZE;
		log_dbg("uninitialized stream {}",GetID());
	}catch(std::exception&exc){
		log_warn(
			"failed to uninitialize stream {}: {}",
			GetID(),exc.what()
		);
		return;
	}
	if(unlikely(status==STREAM_RUNNING)){
		log_warn("stream {} does not stopped",GetID());
		return;
	}
	if(all&&input){
		auto src=GetInputStream();
		log_dbg(
			"stop optional source stream {} requested by {}",
			src->GetID(),GetID()
		);
		src->StopStream(all,deinit);
	}
}
