/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_SERIAL_H
#define GADGET_SERIAL_H
#include"gadget_tty.h"

class GadgetSerial:public GadgetTTY{
	public:GadgetSerial(std::weak_ptr<const Gadget>gadget,const std::string&id);
};;

#endif
