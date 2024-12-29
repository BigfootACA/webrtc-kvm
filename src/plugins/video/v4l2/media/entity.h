/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef V4L2_MEDIA_ENTITY_H
#define V4L2_MEDIA_ENTITY_H
#include<string>
#include<memory>
#include<vector>
#include<linux/media.h>
#include<sys/types.h>

class MediaPad;
class MediaLink;
class MediaTopology;
class MediaInterface;
class MediaEntityDesc;

class MediaEntity{
	public:
		MediaEntity(std::weak_ptr<MediaTopology>topo,size_t i);
		[[nodiscard]] uint32_t GetID()const;
		[[nodiscard]] std::string GetName()const;
		[[nodiscard]] uint32_t GetFunction()const;
		[[nodiscard]] uint32_t GetFlags()const;
		[[nodiscard]] bool HaveFlags(uint32_t flag)const;
		[[nodiscard]] bool HaveFunction(uint32_t func)const;
		[[nodiscard]] bool IsFunction(uint32_t func)const;
		[[nodiscard]] std::shared_ptr<MediaEntityDesc>GetEntityDesc();
		[[nodiscard]] std::vector<dev_t>FindDevices()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaPad>>FindPads()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaInterface>>FindInterfaces()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaLink>>FindInterfaceLinks()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaLink>>FindDataLinks()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaLink>>FindLinksBySource()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaLink>>FindLinksBySink()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaPad>>FindSourcePads()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaPad>>FindSinkPads()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaEntity>>FindSourceEntities()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaEntity>>FindSinkEntities()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaEntity>>FindSourceEntitiesRecursive(uint32_t level=0)const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaEntity>>FindSinkEntitiesRecursive(uint32_t level=0)const;
		[[nodiscard]] std::string ToString()const;
		[[nodiscard]] inline explicit operator std::string()const{return ToString();}
	private:
		[[nodiscard]] media_v2_entity*GetRAW()const;
		[[nodiscard]] std::shared_ptr<MediaTopology>GetTopology()const;
		std::weak_ptr<MediaTopology>topo;
		size_t i;
};

#endif
