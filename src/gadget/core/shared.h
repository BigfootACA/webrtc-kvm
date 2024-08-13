/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef CORE_GADGET_SHARED_H
#define CORE_GADGET_SHARED_H
#include<map>
#include<vector>
#include<string>
#include<memory>
#include<format>
class GadgetConfigString;
class GadgetConfig;
class GadgetString;
class GadgetFunction;
class Gadget;

class GadgetBase{
	public:
		[[nodiscard]] virtual std::string GetType()const=0;
		[[nodiscard]] virtual std::string GetName()const=0;
		[[nodiscard]] virtual std::string GetPath()const=0;
		virtual void Remove();
		void Recreate();
		void Create();
};

#define DECL_ATTR(name,type)\
	[[nodiscard]] type Get##name()const;\
	void Set##name(const type&val);

#endif
