/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"link.h"
#include"topology.h"
#include"interface.h"
#include"lib/strings.h"
#include"lib/exception.h"
#include"defines.h"
#include<sys/sysmacros.h>

std::shared_ptr<MediaInterface>MediaTopology::GetInterface(size_t i)const{
	if(unlikely(i>=v_interfaces.size()))throw InvalidArgument("bad interface index {}",i);
	return v_interfaces[i];
}

std::shared_ptr<MediaInterface>MediaTopology::FindInterfaceByID(uint32_t id)const{
	for(auto&v:v_interfaces)if(v->GetID()==id)return v;
	throw InvalidArgument("interface {:x} not found",id);
}

MediaInterface::MediaInterface(
	std::weak_ptr<MediaTopology>topo,
	size_t i
):topo(std::move(topo)),i(i){}

std::string MediaInterface::ToString()const{
	return std::format(
		"Interface ID: 0x{:x} Type: 0x{:x} Flags: 0x{:x} Device: {}",
		GetID(),GetType(),GetFlags(),DeviceToString(GetDevice())
	);
}

std::shared_ptr<MediaTopology>MediaInterface::GetTopology()const{
	if(auto v=topo.lock())return v;
	throw InvalidArgument("topology gone");
}

media_v2_interface*MediaInterface::GetRAW()const{
	auto topo=GetTopology();
	if(!topo->m_interfaces)throw InvalidArgument("topology is not initialized");
	return &topo->m_interfaces[i];
}

bool MediaInterface::HaveFlags(uint32_t flag)const {
	return have_bit(GetFlags(),flag);
}

uint32_t MediaInterface::GetID()const{
	return GetRAW()->id;
}

uint32_t MediaInterface::GetType()const{
	return GetRAW()->intf_type;
}

uint32_t MediaInterface::GetFlags()const{
	return GetRAW()->flags;
}

dev_t MediaInterface::GetDevice()const{
	auto&d=GetRAW()->devnode;
	return makedev(d.major,d.minor);
}

std::vector<std::shared_ptr<MediaLink>>MediaInterface::FindLinks()const{
	std::vector<std::shared_ptr<MediaLink>>ret;
	for(auto&lnk:GetTopology()->GetLinks()){
		if(!lnk->IsInterfaceLink())continue;
		if(lnk->GetSourceID()==GetID())ret.push_back(lnk);
	}
	return ret;
}

std::vector<std::shared_ptr<MediaEntity>>MediaInterface::FindEntities()const{
	std::vector<std::shared_ptr<MediaEntity>>ret;
	for(auto&lnk:FindLinks()){
		auto ent=std::get<std::shared_ptr<MediaEntity>>(lnk->GetSink());
		ret.push_back(ent);
	}
	return ret;
}
