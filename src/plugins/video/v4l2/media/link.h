/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef V4L2_MEDIA_LINK_H
#define V4L2_MEDIA_LINK_H
#include<string>
#include<memory>
#include<variant>
#include<linux/media.h>

class MediaPad;
class MediaEntity;
class MediaTopology;
class MediaInterface;

class MediaLink{
	public:
		MediaLink(std::weak_ptr<MediaTopology>topo,size_t i);
		[[nodiscard]] uint32_t GetID()const;
		[[nodiscard]] uint32_t GetSourceID()const;
		[[nodiscard]] uint32_t GetSinkID()const;
		[[nodiscard]] uint32_t GetFlags()const;
		[[nodiscard]] std::variant<std::shared_ptr<MediaPad>,std::shared_ptr<MediaInterface>>GetSource()const;
		[[nodiscard]] std::variant<std::shared_ptr<MediaPad>,std::shared_ptr<MediaEntity>>GetSink()const;
		[[nodiscard]] bool HaveFlags(uint32_t flag)const;
		[[nodiscard]] inline uint32_t GetLinkType()const{return GetFlags()&MEDIA_LNK_FL_LINK_TYPE;}
		[[nodiscard]] bool IsDataLink()const{return GetLinkType()==MEDIA_LNK_FL_DATA_LINK;}
		[[nodiscard]] bool IsInterfaceLink()const{return GetLinkType()==MEDIA_LNK_FL_INTERFACE_LINK;}
		[[nodiscard]] std::string ToString()const;
		[[nodiscard]] inline explicit operator std::string()const{return ToString();}
	private:
		[[nodiscard]] media_v2_link*GetRAW()const;
		[[nodiscard]] std::shared_ptr<MediaTopology>GetTopology()const;
		std::weak_ptr<MediaTopology>topo;
		size_t i;
};

#endif
