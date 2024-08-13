/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef CONFIGS_H
#define CONFIGS_H
#include"webrtc_kvm.h"
#include"lib/exception.h"
#include"string/strings.h"
#include<yaml-cpp/yaml.h>

#define load_string_conv(obj,key,dest,type)do{\
	auto _val=obj[#key];\
	if(!_val)break;\
	auto _str=_val.as<std::string>();\
	if(StringTo##type(_str,dest))break;\
	throw InvalidArgument("bad "#type" for "#key": {}",_str);\
}while(0)
#endif
