/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/log.h"

void Stream::StartStream(bool all){
	if(auto src=GetInputStream())if(src->GetStatus()!=STREAM_RUNNING){
		log_dbg(
			"start dependency source stream {} requested by {}",
			src->GetID(),GetID()
		);
		src->StartStream();
		if(unlikely(src->GetStatus()!=STREAM_RUNNING)){
			log_warn(
				"sink stream {} depend on source stream {}, but cannot start it",
				GetID(),src->GetID()
			);
			return;
		}
	}
	if(status==STREAM_UNINITIALIZE)try{
		log_dbg("initializing stream {}",GetID());
		OnInitialize();
		status=STREAM_STOPPED;
		log_dbg("initialized stream {}",GetID());
	}catch(std::exception&exc){
		log_warn(
			"failed to initialize stream {}: {}",
			GetID(),exc.what()
		);
		try{OnDeinitialize();}catch(...){}
		return;
	}
	if(status==STREAM_STOPPED)try{
		log_dbg("starting stream {}",GetID());
		OnStartStream();
		status=STREAM_RUNNING;
		log_dbg("started stream {}",GetID());
	}catch(std::exception&exc){
		log_warn("failed to start stream {}: {}",GetID(),exc.what());
		return;
	}
	if(unlikely(status!=STREAM_RUNNING)){
		log_warn("stream {} does not started",GetID());
		return;
	}
	if(all)if(auto sink=GetOutputStream()){
		log_dbg(
			"start optional sink stream {} requested by {}",
			sink->GetID(),GetID()
		);
		sink->StartStream(all);
	}
}
