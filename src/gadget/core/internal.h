/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef CORE_GADGET_INTERNAL_H
#define CORE_GADGET_INTERNAL_H
#include<format>
#include"shared.h"
#include"lib/log.h"
#include"lib/files.h"
#include"lib/strings.h"

#define VALIDS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_+=.,@"

#define IMPL_ATTR_STRING(cls,name,path)\
	std::string cls::Get##name()const{\
		return ReadFileString(std::format("{}/"#path,GetPath()));\
	}\
	void cls::Set##name(const std::string&val){\
		if(val.empty())log_verb("{} {} clear "#name" value",GetType(),GetName());\
		else log_verb("{} {} set "#name" to {}",GetType(),GetName(),val);\
		WriteFileString(std::format("{}/"#path,GetPath()),val+"\n");\
	}
#define IMPL_ATTR(cls,name,path,rt,type,fmt)\
	type cls::Get##name()const{\
		return (type)ReadFile##rt(std::format("{}/"#path,GetPath()));\
	}\
	void cls::Set##name(const type&val){\
		auto value=std::format(fmt,(type)val);\
		log_verb("{} {} set "#name" to {}",GetType(),GetName(),value);\
		WriteFileString(std::format("{}/"#path,GetPath()),value+"\n");\
	}
#define IMPL_ATTR_HEX(impl,name,path,type,fmt)\
	IMPL_ATTR(impl,name,path,Hex,type,fmt)
#define IMPL_ATTR_UNSIGNED(impl,name,path)\
	IMPL_ATTR(impl,name,path,Unsigned,uint64_t,"{}")
#define IMPL_ATTR_BOOL(impl,name,path)\
	IMPL_ATTR(impl,name,path,Unsigned,bool,"{:d}")
#define IMPL_ATTR_SHEX8(impl,name,path)\
	IMPL_ATTR_HEX(impl,name,path,uint8_t,"0x{:02X}")
#define IMPL_ATTR_HEX8(impl,name,path)\
	IMPL_ATTR_HEX(impl,name,path,uint8_t,"0x{:02X}")
#define IMPL_ATTR_HEX16(impl,name,path)\
	IMPL_ATTR_HEX(impl,name,path,uint16_t,"0x{:04X}")

template<typename S,typename T>
std::vector<std::shared_ptr<T>>ListSubItems(const S*self,const std::string&sub){
	std::vector<std::shared_ptr<T>>res;
	FolderReader d(std::format("{}/{}",self->GetPath(),sub));
	std::shared_ptr<FolderReaderItem>item;
	auto shared=self->shared_from_this();
	while((item=d.ReadItem())){
		if(item->IsHidden()||!item->IsFolder())continue;
		res.push_back(std::make_shared<T>(shared,item->GetName()));
	}
	return res;
}
#endif
