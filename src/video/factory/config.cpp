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

void StreamFactory::CreateStreamsFromConfig(webrtc_kvm*ctx,StreamList*list,YAML::Node&cfg){
	if(!ctx||!list)throw InvalidArgument("bad context or list");
	if(!cfg.IsDefined())return;
	if(!cfg.IsSequence())throw InvalidArgument("config is not list for create streams");
	for(auto&&child:cfg){
		auto stream=CreateStreamFromConfig(ctx,child);
		try{
			auto id=stream->GetID();
			auto uuid=stream->GetUUID();
			auto name=stream->GetName();
			if(list->FindByID(id)||list->FindByUUID(uuid))throw InvalidArgument(
				"stream {} (id {} uuid {}) already exists",name,id,uuid.ToString()
			);
			list->Add(stream);
			log_info("create stream {} id {} uuid {}",name,id,uuid.ToString());
		}catch(...){
			delete stream;
			throw;
		}
	}
}
