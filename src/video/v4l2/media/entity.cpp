/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"pad.h"
#include"link.h"
#include"device.h"
#include"entity.h"
#include"topology.h"
#include"interface.h"
#include"entity_desc.h"
#include"lib/exception.h"
#include"lib/strings.h"
#include"defines.h"

std::shared_ptr<MediaEntity>MediaTopology::GetEntity(size_t i)const{
	if(unlikely(i>=v_entities.size()))throw InvalidArgument("bad entity index {}",i);
	return v_entities[i];
}

std::shared_ptr<MediaEntity>MediaTopology::FindEntityByID(uint32_t id)const{
	for(auto&v:v_entities)if(v->GetID()==id)return v;
	throw InvalidArgument("entity {:x} not found",id);
}

MediaEntity::MediaEntity(
	std::weak_ptr<MediaTopology>topo,
	size_t i
):topo(std::move(topo)),i(i){}

std::string MediaEntity::ToString()const{
	return std::format(
		"Entity ID: 0x{:x} Name: {} Flags: 0x{:x} Function: 0x{:x}",
		GetID(),NonEmpty(GetName()),GetFlags(),GetFunction()
	);
}

std::shared_ptr<MediaTopology>MediaEntity::GetTopology()const{
	if(auto v=topo.lock())return v;
	throw InvalidArgument("topology gone");
}

media_v2_entity*MediaEntity::GetRAW()const{
	auto topo=GetTopology();
	if(!topo->m_entities)throw InvalidArgument("topology is not initialized");
	return &topo->m_entities[i];
}

uint32_t MediaEntity::GetID()const{
	return GetRAW()->id;
}

std::string MediaEntity::GetName()const{
	return CharArrayToString(GetRAW()->name);
}

uint32_t MediaEntity::GetFunction()const{
	return GetRAW()->function;
}

uint32_t MediaEntity::GetFlags()const{
	return GetRAW()->flags;
}

bool MediaEntity::HaveFlags(uint32_t flag)const{
	return have_bit(GetFlags(),flag);
}

bool MediaEntity::HaveFunction(uint32_t func)const{
	return have_bit(GetFunction(),func);
}

bool MediaEntity::IsFunction(uint32_t func)const{
	return GetFunction()==func;
}

std::vector<dev_t>MediaEntity::FindDevices()const{
	std::vector<dev_t>ret;
	for(auto&intf:FindInterfaces())
		ret.push_back(intf->GetDevice());
	return ret;
}

std::vector<std::shared_ptr<MediaPad>>MediaEntity::FindPads()const{
	std::vector<std::shared_ptr<MediaPad>>ret;
	for(auto&pad:GetTopology()->GetPads())
		if(pad->GetEntityID()==GetID())
			ret.push_back(pad);
	return ret;
}

std::vector<std::shared_ptr<MediaInterface>>MediaEntity::FindInterfaces()const{
	std::vector<std::shared_ptr<MediaInterface>>ret;
	for(auto&lnk:GetTopology()->GetLinks()){
		if(!lnk->IsInterfaceLink()||lnk->GetSinkID()!=GetID())continue;
		ret.push_back(std::get<std::shared_ptr<MediaInterface>>(lnk->GetSource()));
	}
	return ret;
}

std::vector<std::shared_ptr<MediaLink>>MediaEntity::FindInterfaceLinks()const{
	std::vector<std::shared_ptr<MediaLink>>ret;
	for(auto&lnk:GetTopology()->GetLinks()){
		if(!lnk->IsInterfaceLink())continue;
		if(lnk->GetSinkID()==GetID())ret.push_back(lnk);
	}
	return ret;
}

std::vector<std::shared_ptr<MediaLink>>MediaEntity::FindDataLinks()const{
	std::vector<std::shared_ptr<MediaLink>>ret;
	for(auto&pad:FindPads()){
		auto lnks=pad->FindLinks();
		ret.insert(ret.end(),lnks.begin(),lnks.end());
	}
	return ret;
}

std::vector<std::shared_ptr<MediaLink>>MediaEntity::FindLinksBySource()const{
	std::vector<std::shared_ptr<MediaLink>>ret;
	for(auto&pad:FindPads()){
		auto lnks=pad->FindLinksBySource();
		ret.insert(ret.end(),lnks.begin(),lnks.end());
	}
	return ret;
}

std::vector<std::shared_ptr<MediaLink>>MediaEntity::FindLinksBySink()const{
	std::vector<std::shared_ptr<MediaLink>>ret;
	for(auto&pad:FindPads()){
		auto lnks=pad->FindLinksBySink();
		ret.insert(ret.end(),lnks.begin(),lnks.end());
	}
	return ret;
}

std::vector<std::shared_ptr<MediaPad>>MediaEntity::FindSourcePads()const{
	std::vector<std::shared_ptr<MediaPad>>ret;
	for(auto&lnk:FindLinksBySink()){
		if(!lnk->IsDataLink())continue;
		ret.push_back(std::get<std::shared_ptr<MediaPad>>(lnk->GetSource()));
	}
	return ret;
}

std::vector<std::shared_ptr<MediaPad>>MediaEntity::FindSinkPads()const{
	std::vector<std::shared_ptr<MediaPad>>ret;
	for(auto&lnk:FindLinksBySource()){
		if(!lnk->IsDataLink())continue;
		ret.push_back(std::get<std::shared_ptr<MediaPad>>(lnk->GetSink()));
	}
	return ret;
}

std::vector<std::shared_ptr<MediaEntity>>MediaEntity::FindSourceEntities()const{
	std::vector<std::shared_ptr<MediaEntity>>ret;
	for(auto&pad:FindSourcePads())
		ret.push_back(pad->GetEntity());
	return ret;
}

std::vector<std::shared_ptr<MediaEntity>>MediaEntity::FindSinkEntities()const{
	std::vector<std::shared_ptr<MediaEntity>>ret;
	for(auto&pad:FindSinkPads())
		ret.push_back(pad->GetEntity());
	return ret;
}

std::vector<std::shared_ptr<MediaEntity>>MediaEntity::FindSourceEntitiesRecursive(uint32_t level)const{
	if(unlikely(level>=64))throw RuntimeError("recursive too deep");
	std::vector<std::shared_ptr<MediaEntity>>ret;
	for(auto&ent:FindSourceEntities()){
		ret.push_back(ent);
		auto s=ent->FindSourceEntitiesRecursive(level+1);
		ret.insert(ret.end(),s.begin(),s.end());
	}
	return ret;
}

std::vector<std::shared_ptr<MediaEntity>>MediaEntity::FindSinkEntitiesRecursive(uint32_t level)const{
	if(unlikely(level>=64))throw RuntimeError("recursive too deep");
	std::vector<std::shared_ptr<MediaEntity>>ret;
	for(auto&ent:FindSinkEntities()){
		ret.push_back(ent);
		auto s=ent->FindSinkEntitiesRecursive(level+1);
		ret.insert(ret.end(),s.begin(),s.end());
	}
	return ret;
}
