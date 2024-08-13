/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef CORE_GADGET_FUNCTION_H
#define CORE_GADGET_FUNCTION_H
#include"shared.h"

class GadgetFunction:public std::enable_shared_from_this<GadgetFunction>,public GadgetBase{
	public:
		GadgetFunction(std::weak_ptr<const Gadget>gadget,const std::string&func,const std::string&id);
		virtual ~GadgetFunction();
		[[nodiscard]] std::string GetPath()const override;
		[[nodiscard]] inline std::weak_ptr<const Gadget>GetParent()const{return gadget;}
		[[nodiscard]] std::map<std::shared_ptr<GadgetConfig>,std::string>ListLinkedConfigs()const;
		void LinkToConfig(const std::shared_ptr<const GadgetConfig>&cfg,const std::string&link_name)const;
		[[nodiscard]] inline std::string GetName()const override{return std::format("{}.{}",func,id);}
		[[nodiscard]] inline std::string GetType()const override{return "gadget function";}
		[[nodiscard]] inline std::string GetFunction()const{return func;}
		[[nodiscard]] inline std::string GetID()const{return id;}
		virtual void Remove()override;
		static std::shared_ptr<GadgetFunction>FromName(const std::weak_ptr<const Gadget>&gadget,const std::string&name);
	protected:
		std::weak_ptr<const Gadget>gadget;
		std::string func;
		std::string id;
};

#endif
