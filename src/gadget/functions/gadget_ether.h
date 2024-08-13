/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_ETHER_H
#define GADGET_ETHER_H
#include"gadget_net.h"

class GadgetEthernet:public GadgetNet{
	public:GadgetEthernet(std::weak_ptr<const Gadget>gadget,const std::string&id);
};

#endif
