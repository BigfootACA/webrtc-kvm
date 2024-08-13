/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_EEM_H
#define GADGET_EEM_H
#include"gadget_net.h"

class GadgetEEM:public GadgetNet{
	public:GadgetEEM(std::weak_ptr<const Gadget>gadget,const std::string&id);
};

#endif
