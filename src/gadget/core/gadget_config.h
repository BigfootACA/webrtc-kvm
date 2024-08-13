/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef CORE_GADGET_CONFIG_H
#define CORE_GADGET_CONFIG_H
#include"shared.h"

class GadgetConfig:public std::enable_shared_from_this<GadgetConfig>,public GadgetBase{
	public:
		GadgetConfig(std::weak_ptr<const Gadget>gadget,const std::string&name);
		GadgetConfig(std::weak_ptr<const Gadget>gadget,const std::string&id,uint16_t num);
		virtual ~GadgetConfig();
		DECL_ATTR(BmAttributes,uint8_t);
		DECL_ATTR(MaxPower,uint64_t);
		[[nodiscard]] std::string GetPath()const override;
		[[nodiscard]] std::vector<std::shared_ptr<GadgetConfigString>>ListStrings()const;
		[[nodiscard]] std::map<std::string,std::shared_ptr<GadgetFunction>>ListLinkedFunctions()const;
		[[nodiscard]] std::string GetConfiguration(uint16_t lang);
		std::shared_ptr<GadgetConfigString>AddString(uint16_t lang,const std::string&configuration="");
		[[nodiscard]] inline std::weak_ptr<const Gadget>GetParent()const{return gadget;}
		[[nodiscard]] inline std::string GetName()const override{return name;}
		[[nodiscard]] inline std::string GetID()const{return id;}
		[[nodiscard]] inline uint16_t GetNum()const{return num;}
		[[nodiscard]] inline std::string GetType()const override{return "gadget config";}
		void LinkFunction(const std::shared_ptr<const GadgetFunction>&func,const std::string&name="")const;
		void RemoveLinkedFunction(const std::string&name);
		void RemoveLinkedFunction(const std::shared_ptr<GadgetFunction>&func);
		void Remove()override;
	private:
		const std::weak_ptr<const Gadget>gadget;
		std::string name;
		std::string id;
		uint8_t num;
};

#endif
