/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_ACM_H
#define GADGET_ACM_H
#include"gadget_tty.h"

class GadgetACM:public GadgetTTY{
	public:GadgetACM(std::weak_ptr<const Gadget>gadget,const std::string&id);
};

#endif
