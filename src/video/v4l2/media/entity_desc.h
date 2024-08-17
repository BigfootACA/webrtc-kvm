/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef V4L2_MEDIA_ENTITY_DESC_H
#define V4L2_MEDIA_ENTITY_DESC_H
#include<string>
#include<memory>
#include<linux/media.h>

class MediaDevice;
class MediaEntity;

class MediaEntityDesc:public std::enable_shared_from_this<MediaEntityDesc>{
	public:
		MediaEntityDesc(std::weak_ptr<MediaDevice>dev,const media_entity_desc&desc);
		[[nodiscard]] std::string GetName()const;
		[[nodiscard]] inline uint32_t GetID()const{return desc.id;}
		[[nodiscard]] inline uint32_t GetType()const{return desc.type;}
		[[nodiscard]] inline uint32_t GetRevision()const{return desc.revision;}
		[[nodiscard]] inline uint32_t GetFlags()const{return desc.flags;}
		[[nodiscard]] inline uint32_t GetGroupID()const{return desc.group_id;}
		[[nodiscard]] inline uint32_t GetPads()const{return desc.pads;}
		[[nodiscard]] inline uint32_t GetLinks()const{return desc.links;}
		[[nodiscard]] bool HaveFlags(uint32_t flag)const;
		[[nodiscard]] bool HaveType(uint32_t type)const;
		[[nodiscard]] bool IsType(uint32_t type)const;
		[[nodiscard]] std::shared_ptr<MediaEntity>GetEntity()const;
		[[nodiscard]] std::string ToString()const;
		[[nodiscard]] inline explicit operator std::string()const{return ToString();}
	protected:
		[[nodiscard]] std::shared_ptr<MediaDevice>GetDevice()const;
		std::weak_ptr<MediaDevice>dev;
		media_entity_desc desc;
};

#endif
