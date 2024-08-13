/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_MASS_H
#define GADGET_MASS_H
#include"../core/gadget_function.h"

class GadgetMassStorage:public GadgetFunction{
	public:
		GadgetMassStorage(std::weak_ptr<const Gadget>gadget,const std::string&id);
		DECL_ATTR(Stall,bool);
};

#endif
