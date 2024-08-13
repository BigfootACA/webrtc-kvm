/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_FFS_H
#define GADGET_FFS_H
#include"../core/gadget_function.h"

class GadgetFunctionFS:public GadgetFunction{
	public:GadgetFunctionFS(std::weak_ptr<const Gadget>gadget,const std::string&id);
};

#endif
