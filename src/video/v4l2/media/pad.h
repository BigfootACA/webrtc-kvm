/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef V4L2_MEDIA_PAD_H
#define V4L2_MEDIA_PAD_H
#include<string>
#include<memory>
#include<vector>
#include<linux/media.h>

class MediaLink;
class MediaEntity;
class MediaTopology;

class MediaPad{
	public:
		MediaPad(std::weak_ptr<MediaTopology>topo,size_t i);
		[[nodiscard]] uint32_t GetID()const;
		[[nodiscard]] uint32_t GetEntityID()const;
		[[nodiscard]] uint32_t GetFlags()const;
		[[nodiscard]] uint32_t GetIndex()const;
		[[nodiscard]] std::shared_ptr<MediaEntity> GetEntity()const;
		[[nodiscard]] bool HaveFlags(uint32_t flag)const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaLink>>FindLinks()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaLink>>FindLinksBySource()const;
		[[nodiscard]] std::vector<std::shared_ptr<MediaLink>>FindLinksBySink()const;
		[[nodiscard]] std::string ToString()const;
		[[nodiscard]] inline explicit operator std::string()const{return ToString();}
	private:
		[[nodiscard]] media_v2_pad*GetRAW()const;
		[[nodiscard]] std::shared_ptr<MediaTopology>GetTopology()const;
		std::weak_ptr<MediaTopology>topo;
		size_t i;
};

#endif
