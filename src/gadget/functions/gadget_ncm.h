/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_NCM_H
#define GADGET_NCM_H
#include"gadget_net.h"

class GadgetNCM:public GadgetNet{
	public:
		GadgetNCM(std::weak_ptr<const Gadget>gadget,const std::string&id);
};

#endif
