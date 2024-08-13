/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef CORE_GADGET_STRING_H
#define CORE_GADGET_STRING_H
#include"shared.h"

class GadgetString:public std::enable_shared_from_this<GadgetString>,public GadgetBase{
	public:
		GadgetString(std::weak_ptr<const Gadget>gadget,const std::string&name);
		GadgetString(std::weak_ptr<const Gadget>gadget,uint16_t id);
		virtual ~GadgetString();
		DECL_ATTR(Manufacturer,std::string);
		DECL_ATTR(Product,std::string);
		DECL_ATTR(SerialNumber,std::string);
		[[nodiscard]] std::string GetPath()const override;
		[[nodiscard]] inline std::weak_ptr<const Gadget>GetParent()const{return gadget;}
		[[nodiscard]] inline std::string GetName()const override{return name;}
		[[nodiscard]] inline std::string GetType()const override{return "gadget string";}
		[[nodiscard]] inline uint16_t GetID()const{return id;}
	private:
		std::weak_ptr<const Gadget>gadget;
		std::string name;
		uint16_t id;
};

#endif
