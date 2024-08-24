/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"device.h"
#include"entity.h"
#include"topology.h"
#include"entity_desc.h"
#include"lib/exception.h"
#include"lib/strings.h"
#include"webrtc_kvm.h"

MediaEntityDesc::MediaEntityDesc(
	std::weak_ptr<MediaDevice>dev,
	const media_entity_desc&desc
):dev(std::move(dev)),desc(desc){}

std::string MediaEntityDesc::GetName()const{
	return CharArrayToString(desc.name);
}

std::string MediaEntityDesc::ToString()const{
	return std::format(
		"Entity desc ID: 0x{:x} Type: 0x{:x} Revision: 0x{:x} "
		"Name: {} Flags: 0x{:x} Group: 0x{:x} Pads: {} Links: {}",
		GetID(),GetType(),GetRevision(),NonEmpty(GetName()),
		GetFlags(),GetGroupID(),GetPads(),GetLinks()
	);
}

std::shared_ptr<MediaDevice>MediaEntityDesc::GetDevice()const{
	if(auto v=dev.lock())return v;
	throw InvalidArgument("device gone");
}

bool MediaEntityDesc::HaveFlags(uint32_t flag)const{
	return have_bit(GetFlags(),flag);
}

bool MediaEntityDesc::HaveType(uint32_t type)const{
	return have_bit(GetType(),type);
}

bool MediaEntityDesc::IsType(uint32_t type)const{
	return GetType()==type;
}

std::shared_ptr<MediaEntity>MediaEntityDesc::GetEntity()const{
	return GetDevice()->GetTopology()->FindEntityByID(GetID());
}

std::shared_ptr<MediaEntityDesc>MediaEntity::GetEntityDesc(){
	return GetTopology()->GetDevice()->EnumEntity(GetID());
}

std::vector<std::shared_ptr<MediaEntityDesc>>MediaDevice::EnumEntities(){
	media_entity_desc desc{};
	auto self=shared_from_this();
	std::vector<std::shared_ptr<MediaEntityDesc>>ret;
	while(true)try{
		desc.id|=MEDIA_ENT_ID_FLAG_NEXT;
		xioctl(fd,MEDIA_IOC_ENUM_ENTITIES,&desc);
		auto d=std::make_shared<MediaEntityDesc>(self,desc);
		ret.push_back(d);
	}catch(Exceptions::ErrnoExceptionImpl&exc){
		if(exc.err==EINVAL)break;
		throw;
	}
	return ret;
}

std::shared_ptr<MediaEntityDesc>MediaDevice::EnumEntity(uint32_t id){
	media_entity_desc desc{};
	auto self=shared_from_this();
	try{
		desc.id=id;
		xioctl(fd,MEDIA_IOC_ENUM_ENTITIES,&desc);
		auto d=std::make_shared<MediaEntityDesc>(self,desc);
		return d;
	}catch(Exceptions::ErrnoExceptionImpl&exc){
		if(exc.err==EINVAL)return nullptr;
		throw;
	}
}
