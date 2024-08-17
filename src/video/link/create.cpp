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

std::string StreamLink::ToString()const{
	return std::format(
		"{} <=> {}",
		source?source->GetID():"(null)",
		sink?sink->GetID():"(null)"
	);
}

void StreamLink::CreateLinksFromConfig(StreamList*list,YAML::Node&cfg){
	if(!list)throw InvalidArgument("bad list");
	if(!cfg.IsDefined())return;
	if(!cfg.IsSequence())throw InvalidArgument("config is not list for create links");
	for(auto&&i:cfg){
		auto child=i;
		auto link=std::make_shared<StreamLink>();
		if(!link)throw InvalidArgument("bad stream link");
		link->Initialize(list,child);
		log_info("create stream link {}",link->ToString());
	}
}
