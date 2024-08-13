/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_ECM_H
#define GADGET_ECM_H
#include"gadget_net.h"

class GadgetECM:public GadgetNet{
	public:GadgetECM(std::weak_ptr<const Gadget>gadget,const std::string&id);
};

#endif
