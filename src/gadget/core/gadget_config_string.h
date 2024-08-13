/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef CORE_GADGET_CONFIG_STRING_H
#define CORE_GADGET_CONFIG_STRING_H
#include"shared.h"

class GadgetConfigString:public std::enable_shared_from_this<GadgetConfigString>,public GadgetBase{
	public:
		GadgetConfigString(std::weak_ptr<const GadgetConfig>config,const std::string&name);
		GadgetConfigString(std::weak_ptr<const GadgetConfig>config,uint16_t id);
		virtual ~GadgetConfigString();
		DECL_ATTR(Configuration,std::string);
		[[nodiscard]] std::string GetPath()const override;
		[[nodiscard]] inline std::weak_ptr<const GadgetConfig>GetParent()const{return config;}
		[[nodiscard]] inline std::string GetName()const override{return name;}
		[[nodiscard]] inline uint16_t GetID()const{return id;}
		[[nodiscard]] inline std::string GetType()const override{return "gadget config string";}
	private:
		std::weak_ptr<const GadgetConfig>config;
		std::string name;
		uint16_t id;
};

#endif
