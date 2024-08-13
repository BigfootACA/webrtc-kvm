/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_NET_H
#define GADGET_NET_H
#include"../core/gadget_function.h"

class GadgetNet:public GadgetFunction{
	public:
		inline GadgetNet(
			std::weak_ptr<const Gadget>gadget,
			const std::string&func,
			const std::string&id
		):GadgetFunction(std::move(gadget),func,id){}
		DECL_ATTR(DeviceAddress,std::string)
		DECL_ATTR(HostAddress,std::string)
		DECL_ATTR(InterfaceName,std::string)
		DECL_ATTR(QueueLengthMultiplier,uint64_t)
};

#endif
