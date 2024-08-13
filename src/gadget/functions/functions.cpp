/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../core/internal.h"
#include"../core/gadget.h"
#include"gadget_acm.h"
#include"gadget_ecm.h"
#include"gadget_eem.h"
#include"gadget_ether.h"
#include"gadget_ffs.h"
#include"gadget_hid.h"
#include"gadget_mass.h"
#include"gadget_ncm.h"
#include"gadget_net.h"
#include"gadget_rndis.h"
#include"gadget_serial.h"
#include"gadget_tty.h"
#include<inttypes.h>

#define IMPL_CONSTRUCTOR(par,cls,func)cls::cls(std::weak_ptr<const Gadget>gadget,const std::string&id):par(std::move(gadget),#func,id){}
IMPL_CONSTRUCTOR(GadgetFunction,GadgetFunctionFS,ffs)
IMPL_CONSTRUCTOR(GadgetFunction,GadgetHID,hid)
IMPL_CONSTRUCTOR(GadgetFunction,GadgetMassStorage,mass_storage)
IMPL_CONSTRUCTOR(GadgetTTY,GadgetACM,acm)
IMPL_CONSTRUCTOR(GadgetTTY,GadgetSerial,gser)
IMPL_CONSTRUCTOR(GadgetNet,GadgetRNDIS,rndis)
IMPL_CONSTRUCTOR(GadgetNet,GadgetECM,ecm)
IMPL_CONSTRUCTOR(GadgetNet,GadgetEEM,eem)
IMPL_CONSTRUCTOR(GadgetNet,GadgetNCM,ncm)
IMPL_CONSTRUCTOR(GadgetNet,GadgetEthernet,geth)
IMPL_ATTR_STRING(GadgetNet,DeviceAddress,dev_addr)
IMPL_ATTR_STRING(GadgetNet,HostAddress,host_addr)
IMPL_ATTR_STRING(GadgetNet,InterfaceName,ifname)
IMPL_ATTR_UNSIGNED(GadgetNet,QueueLengthMultiplier,qmult)
IMPL_ATTR_BOOL(GadgetTTY,Console,console)
IMPL_ATTR_UNSIGNED(GadgetTTY,PortNumber,port_num)
IMPL_ATTR_BOOL(GadgetMassStorage,Stall,stall)
IMPL_ATTR_HEX8(GadgetRNDIS,DeviceClass,class)
IMPL_ATTR_HEX8(GadgetRNDIS,DeviceProtocol,protocol)
IMPL_ATTR_HEX8(GadgetRNDIS,DeviceSubClass,subclass)

std::shared_ptr<GadgetFunction>GadgetFunction::FromName(const std::weak_ptr<const Gadget>&gadget,const std::string&name){
	auto i=name.find('.');
	if(i==std::string::npos)return nullptr;
	auto func=name.substr(0,i);
	auto id=name.substr(i+1);
	if(func=="ffs")return std::make_shared<GadgetFunctionFS>(gadget,id);
	else if(func=="hid")return std::make_shared<GadgetHID>(gadget,id);
	else if(func=="acm")return std::make_shared<GadgetACM>(gadget,id);
	else if(func=="ecm")return std::make_shared<GadgetECM>(gadget,id);
	else if(func=="eem")return std::make_shared<GadgetEEM>(gadget,id);
	else if(func=="ncm")return std::make_shared<GadgetNCM>(gadget,id);
	else if(func=="gser")return std::make_shared<GadgetSerial>(gadget,id);
	else if(func=="geth")return std::make_shared<GadgetEthernet>(gadget,id);
	else if(func=="rndis")return std::make_shared<GadgetRNDIS>(gadget,id);
	else if(func=="mass_storage")return std::make_shared<GadgetMassStorage>(gadget,id);
	else return std::make_shared<GadgetFunction>(gadget,func,id);
}
