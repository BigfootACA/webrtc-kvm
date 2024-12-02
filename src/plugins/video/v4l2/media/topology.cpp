/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"pad.h"
#include"link.h"
#include"entity.h"
#include"device.h"
#include"topology.h"
#include"interface.h"
#include"interface.h"
#include"lib/exception.h"
#include"webrtc_kvm.h"

void MediaTopology::Load(){
	Unload();
	auto fd=GetDevice()->fd;
	xioctl(fd,MEDIA_IOC_G_TOPOLOGY,&topo);
	m_pads=new media_v2_pad[topo.num_pads];
	m_links=new media_v2_link[topo.num_links];
	m_entities=new media_v2_entity[topo.num_entities];
	m_interfaces=new media_v2_interface[topo.num_interfaces];
	topo.ptr_pads=reinterpret_cast<uint64_t>(m_pads);
	topo.ptr_links=reinterpret_cast<uint64_t>(m_links);
	topo.ptr_entities=reinterpret_cast<uint64_t>(m_entities);
	topo.ptr_interfaces=reinterpret_cast<uint64_t>(m_interfaces);
	xioctl(fd,MEDIA_IOC_G_TOPOLOGY,&topo);
	auto self=shared_from_this();
	for(size_t i=0;i<topo.num_pads;i++){
		auto v=std::make_shared<MediaPad>(self,i);
		v_pads.push_back(v);
	}
	for(size_t i=0;i<topo.num_links;i++){
		auto v=std::make_shared<MediaLink>(self,i);
		v_links.push_back(v);
	}
	for(size_t i=0;i<topo.num_entities;i++){
		auto v=std::make_shared<MediaEntity>(self,i);
		v_entities.push_back(v);
	}
	for(size_t i=0;i<topo.num_interfaces;i++){
		auto v=std::make_shared<MediaInterface>(self,i);
		v_interfaces.push_back(v);
	}
}

void MediaTopology::Unload(){
	memset(&topo,0,sizeof(topo));
	delete[]m_pads;
	delete[]m_links;
	delete[]m_entities;
	delete[]m_interfaces;
	v_pads.clear();
	v_links.clear();
	v_entities.clear();
	v_interfaces.clear();
}

std::vector<std::shared_ptr<MediaInterface>>MediaTopology::FindInterfacesByDevice(dev_t dev)const{
	std::vector<std::shared_ptr<MediaInterface>>ret;
	for(auto&intf:GetInterfaces())
		if(intf->GetDevice()==dev)
			ret.push_back(intf);
	return ret;
}

std::vector<std::shared_ptr<MediaLink>>MediaTopology::FindLinksByDevice(dev_t dev)const{
	std::vector<std::shared_ptr<MediaLink>>ret;
	for(auto&intf:FindInterfacesByDevice(dev)){
		auto lnks=intf->FindLinks();
		ret.insert(ret.end(),lnks.begin(),lnks.end());
	}
	return ret;
}

std::vector<std::shared_ptr<MediaEntity>>MediaTopology::FindEntitiesByDevice(dev_t dev)const{
	std::vector<std::shared_ptr<MediaEntity>>ret;
	for(auto&intf:FindInterfacesByDevice(dev)){
		auto entities=intf->FindEntities();
		ret.insert(ret.end(),entities.begin(),entities.end());
	}
	return ret;
}

std::shared_ptr<MediaInterface>MediaTopology::FindInterfaceByDevice(dev_t dev)const{
	auto v=FindInterfacesByDevice(dev);
	return v.empty()?nullptr:v[0];
}


std::shared_ptr<MediaLink>MediaTopology::FindLinkByDevice(dev_t dev)const{
	auto v=FindLinksByDevice(dev);
	return v.empty()?nullptr:v[0];
}


std::shared_ptr<MediaEntity>MediaTopology::FindEntityByDevice(dev_t dev)const{
	auto v=FindEntitiesByDevice(dev);
	return v.empty()?nullptr:v[0];
}

std::shared_ptr<MediaDevice>MediaTopology::GetDevice()const{
	if(auto v=dev.lock())return v;
	throw InvalidArgument("device gone");
}

std::shared_ptr<MediaTopology>MediaDevice::GetTopology(){
	auto topo=std::make_shared<MediaTopology>(shared_from_this());
	topo->Load();
	return topo;
}
