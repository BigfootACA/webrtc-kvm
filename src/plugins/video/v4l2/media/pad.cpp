/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"pad.h"
#include"link.h"
#include"topology.h"
#include"lib/exception.h"
#include"defines.h"

std::shared_ptr<MediaPad>MediaTopology::GetPad(size_t i)const{
	if(unlikely(i>=v_pads.size()))throw InvalidArgument("bad pad index {}",i);
	return v_pads[i];
}

std::shared_ptr<MediaPad>MediaTopology::FindPadByID(uint32_t id)const{
	for(auto&v:v_pads)if(v->GetID()==id)return v;
	throw InvalidArgument("pad {:x} not found",id);
}

MediaPad::MediaPad(
	std::weak_ptr<MediaTopology>topo,
	size_t i
):topo(std::move(topo)),i(i){}

std::string MediaPad::ToString()const{
	return std::format(
		"Pad ID: 0x{:x} Entity: 0x{:x} Flags: 0x{:x} Index: 0x{:x}",
		GetID(),GetEntityID(),GetFlags(),GetIndex()
	);
}

std::shared_ptr<MediaTopology>MediaPad::GetTopology()const{
	if(auto v=topo.lock())return v;
	throw InvalidArgument("topology gone");
}

media_v2_pad*MediaPad::GetRAW()const{
	auto topo=GetTopology();
	if(!topo->m_pads)throw InvalidArgument("topology is not initialized");
	return &topo->m_pads[i];
}

uint32_t MediaPad::GetID()const{
	return GetRAW()->id;
}

uint32_t MediaPad::GetEntityID()const{
	return GetRAW()->entity_id;
}

uint32_t MediaPad::GetFlags()const{
	return GetRAW()->flags;
}

uint32_t MediaPad::GetIndex()const{
	return GetRAW()->index;
}

std::shared_ptr<MediaEntity>MediaPad::GetEntity()const{
	return GetTopology()->FindEntityByID(GetEntityID());
}

bool MediaPad::HaveFlags(uint32_t flag)const{
	return have_bit(GetFlags(),flag);
}

std::vector<std::shared_ptr<MediaLink>>MediaPad::FindLinks()const{
	std::vector<std::shared_ptr<MediaLink>>ret;
	for(auto&lnk:GetTopology()->GetLinks()){
		if(!lnk->IsDataLink())continue;
		if(
			lnk->GetSinkID()==GetID()||
			lnk->GetSourceID()==GetID()
		)ret.push_back(lnk);
	}
	return ret;
}

std::vector<std::shared_ptr<MediaLink>>MediaPad::FindLinksBySource()const{
	std::vector<std::shared_ptr<MediaLink>>ret;
	for(auto&lnk:GetTopology()->GetLinks()){
		if(!lnk->IsDataLink())continue;
		if(lnk->GetSourceID()==GetID())ret.push_back(lnk);
	}
	return ret;
}

std::vector<std::shared_ptr<MediaLink>>MediaPad::FindLinksBySink()const{
	std::vector<std::shared_ptr<MediaLink>>ret;
	for(auto&lnk:GetTopology()->GetLinks()){
		if(!lnk->IsDataLink())continue;
		if(lnk->GetSinkID()==GetID())ret.push_back(lnk);
	}
	return ret;
}
