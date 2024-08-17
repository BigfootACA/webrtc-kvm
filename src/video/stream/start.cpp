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

void Stream::StartStream(bool all){
	if(input)if(auto src=GetInputStream())if(src->GetStatus()!=STREAM_RUNNING){
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
		switch(GetType()){
			case STREAM_SOURCE:if(!output)throw InvalidArgument("no output for stream source");break;
			case STREAM_PIPE:if(!input||!output)throw InvalidArgument("no input or output for stream pipe");break;
			case STREAM_SINK:if(!input)throw InvalidArgument("no input for stream sink");break;
			default:throw InvalidArgument("unknown stream type");
		}
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
	if(all&&output){
		auto sink=GetOutputStream();
		log_dbg(
			"start optional sink stream {} requested by {}",
			sink->GetID(),GetID()
		);
		sink->StartStream(all);
	}
}
