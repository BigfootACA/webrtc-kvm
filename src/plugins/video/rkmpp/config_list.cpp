/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"
#include"lib/log.h"
#include"lib/exception.h"

void MppConfigList::AddS32(const std::string&key,int32_t v){
	MppConfigItem d={.key=key,.type=MppConfigItem::TYPE_S32,.v={.s32=v}};
	config[key]=d;
}

void MppConfigList::AddU32(const std::string&key,uint32_t v){
	MppConfigItem d={.key=key,.type=MppConfigItem::TYPE_U32,.v={.u32=v}};
	config[key]=d;
}

void MppConfigList::AddS64(const std::string&key,int64_t v){
	MppConfigItem d={.key=key,.type=MppConfigItem::TYPE_S64,.v={.s64=v}};
	config[key]=d;
}

void MppConfigList::AddU64(const std::string&key,uint64_t v){
	MppConfigItem d={.key=key,.type=MppConfigItem::TYPE_U64,.v={.u64=v}};
	config[key]=d;
}

void MppConfigList::FillToEncoder(MppEncCfg cfg){
	MPP_RET ret;
	if(config.empty())return;
	for(const auto&item:config){
		auto&v=item.second.v;
		auto name=item.second.key.c_str();
		switch(item.second.type){
			case MppConfigItem::TYPE_S32:ret=mpp_enc_cfg_set_s32(cfg,name,v.s32);break;
			case MppConfigItem::TYPE_U32:ret=mpp_enc_cfg_set_u32(cfg,name,v.u32);break;
			case MppConfigItem::TYPE_S64:ret=mpp_enc_cfg_set_s64(cfg,name,v.s64);break;
			case MppConfigItem::TYPE_U64:ret=mpp_enc_cfg_set_u64(cfg,name,v.u64);break;
			default:throw InvalidArgument(
				"unsupported config type {} in {}",
				(int)item.second.type,item.second.key
			);
		}
		if(ret!=MPP_OK)throw RuntimeError(
			"mpp change encoder config {} failed: {}",
			item.second.key,(int)ret
		);
	}
}

void MppConfigList::FromYAML(YAML::Node&cfg){
	if(!cfg.IsDefined())return;
	if(!cfg.IsMap())throw InvalidArgument("bad rkmpp config");
	for(const auto&kv:cfg){
		auto name=kv.first.as<std::string>();
		if(name.starts_with("s32:"))AddS32(name.substr(4),kv.second.as<int32_t>());
		else if(name.starts_with("u32:"))AddU32(name.substr(4),kv.second.as<uint32_t>());
		else if(name.starts_with("s64:"))AddS64(name.substr(4),kv.second.as<int64_t>());
		else if(name.starts_with("u64:"))AddU64(name.substr(4),kv.second.as<uint64_t>());
		else throw InvalidArgument("unsupported key {}",name);
	}
}
