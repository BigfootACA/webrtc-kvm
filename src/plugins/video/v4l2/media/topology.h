/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef V4L2_MEDIA_TOPOLOGY_H
#define V4L2_MEDIA_TOPOLOGY_H
#include<memory>
#include<utility>
#include<vector>
#include<linux/media.h>
#include<sys/types.h>

class MediaPad;
class MediaLink;
class MediaDevice;
class MediaEntity;
class MediaInterface;

class MediaTopology:public std::enable_shared_from_this<MediaTopology>{
	friend class MediaPad;
	friend class MediaLink;
	friend class MediaEntity;
	friend class MediaDevice;
	friend class MediaInterface;
	public:
		inline explicit MediaTopology(std::weak_ptr<MediaDevice>dev):dev(std::move(dev)){}
		inline ~MediaTopology(){Unload();}
		[[nodiscard]] inline uint64_t GetVersion()const{return topo.topology_version;}
		[[nodiscard]] inline std::vector<std::shared_ptr<MediaPad>>GetPads()const{return v_pads;}
		[[nodiscard]] inline std::vector<std::shared_ptr<MediaLink>>GetLinks()const{return v_links;}
		[[nodiscard]] inline std::vector<std::shared_ptr<MediaEntity>>GetEntities()const{return v_entities;}
		[[nodiscard]] inline std::vector<std::shared_ptr<MediaInterface>>GetInterfaces()const{return v_interfaces;}
		[[nodiscard]] std::shared_ptr<MediaPad>GetPad(size_t i)const;
		[[nodiscard]] std::shared_ptr<MediaLink>GetLink(size_t i)const;
		[[nodiscard]] std::shared_ptr<MediaEntity>GetEntity(size_t i)const;
		[[nodiscard]] std::shared_ptr<MediaInterface>GetInterface(size_t i)const;
		[[nodiscard]] std::shared_ptr<MediaPad>FindPadByID(uint32_t id)const;
		[[nodiscard]] std::shared_ptr<MediaLink>FindLinkByID(uint32_t id)const;
		[[nodiscard]] std::shared_ptr<MediaEntity>FindEntityByID(uint32_t id)const;
		[[nodiscard]] std::shared_ptr<MediaInterface>FindInterfaceByID(uint32_t id)const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaInterface>>FindInterfacesByDevice(dev_t dev)const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaLink>>FindLinksByDevice(dev_t dev)const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaEntity>>FindEntitiesByDevice(dev_t dev)const;
		[[nodiscard]] std::shared_ptr<MediaInterface>FindInterfaceByDevice(dev_t dev)const;
		[[nodiscard]] std::shared_ptr<MediaLink>FindLinkByDevice(dev_t dev)const;
		[[nodiscard]] std::shared_ptr<MediaEntity>FindEntityByDevice(dev_t dev)const;
	protected:
		[[nodiscard]] std::shared_ptr<MediaDevice>GetDevice()const;
		void Load();
		void Unload();
		media_v2_topology topo{};
		media_v2_pad*m_pads=nullptr;
		media_v2_link*m_links=nullptr;
		media_v2_entity*m_entities=nullptr;
		media_v2_interface*m_interfaces=nullptr;
		std::vector<std::shared_ptr<MediaPad>>v_pads;
		std::vector<std::shared_ptr<MediaLink>>v_links;
		std::vector<std::shared_ptr<MediaEntity>>v_entities;
		std::vector<std::shared_ptr<MediaInterface>>v_interfaces;
		std::weak_ptr<MediaDevice>dev;
};

#endif
