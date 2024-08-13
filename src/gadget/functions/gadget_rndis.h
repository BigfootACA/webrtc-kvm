/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_RNDIS_H
#define GADGET_RNDIS_H
#include"gadget_net.h"

class GadgetRNDIS:public GadgetNet{
	public:
		GadgetRNDIS(std::weak_ptr<const Gadget>gadget,const std::string&id);
		DECL_ATTR(DeviceClass,uint8_t);
		DECL_ATTR(DeviceProtocol,uint8_t);
		DECL_ATTR(DeviceSubClass,uint8_t);
};

#endif
