/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"link.h"
#include"topology.h"
#include"lib/exception.h"
#include"defines.h"

std::shared_ptr<MediaLink>MediaTopology::GetLink(size_t i)const{
	if(unlikely(i>=v_links.size()))throw InvalidArgument("bad link index {}",i);
	return v_links[i];
}

std::shared_ptr<MediaLink>MediaTopology::FindLinkByID(uint32_t id)const{
	for(auto&v:v_links)if(v->GetID()==id)return v;
	throw InvalidArgument("link {:x} not found",id);
}

MediaLink::MediaLink(
	std::weak_ptr<MediaTopology>topo,
	size_t i
):topo(std::move(topo)),i(i){}

std::string MediaLink::ToString()const{
	return std::format(
		"Link ID: 0x{:x} Source: 0x{:x} Sink: 0x{:x} Flags: 0x{:x}",
		GetID(),GetSourceID(),GetSinkID(),GetFlags()
	);
}

std::shared_ptr<MediaTopology>MediaLink::GetTopology()const{
	if(auto v=topo.lock())return v;
	throw InvalidArgument("topology gone");
}

media_v2_link*MediaLink::GetRAW()const{
	auto topo=GetTopology();
	if(!topo->m_links)throw InvalidArgument("topology is not initialized");
	return &topo->m_links[i];
}

uint32_t MediaLink::GetID()const{
	return GetRAW()->id;
}

uint32_t MediaLink::GetSourceID()const{
	return GetRAW()->source_id;
}

uint32_t MediaLink::GetSinkID()const{
	return GetRAW()->sink_id;
}

uint32_t MediaLink::GetFlags()const{
	return GetRAW()->flags;
}

bool MediaLink::HaveFlags(uint32_t flag)const{
	return have_bit(GetFlags(),flag);
}

std::variant<std::shared_ptr<MediaPad>,std::shared_ptr<MediaInterface>>MediaLink::GetSource()const{
	auto p=GetTopology();
	auto id=GetSourceID();
	if(IsInterfaceLink())return p->FindInterfaceByID(id);
	return p->FindPadByID(id);
}

std::variant<std::shared_ptr<MediaPad>,std::shared_ptr<MediaEntity>>MediaLink::GetSink()const{
	auto p=GetTopology();
	auto id=GetSinkID();
	if(IsInterfaceLink())return p->FindEntityByID(id);
	return p->FindPadByID(id);
}
