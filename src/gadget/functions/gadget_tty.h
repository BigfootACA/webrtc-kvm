/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_TTY_H
#define GADGET_TTY_H
#include"../core/gadget_function.h"

class GadgetTTY:public GadgetFunction{
	public:
		inline GadgetTTY(
			std::weak_ptr<const Gadget>gadget,
			const std::string&func,
			const std::string&id
		):GadgetFunction(std::move(gadget),func,id){}
		DECL_ATTR(Console,bool)
		DECL_ATTR(PortNumber,uint64_t)
};


#endif
