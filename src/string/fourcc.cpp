/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"strings.h"
#include<string>
#include<cstdint>
#include<cstring>
#include<format>
#include<cctype>

bool StringToFourcc(const std::string&val,uint32_t&fourcc){
	if(val.length()!=4)return false;
	memcpy(&fourcc,val.c_str(),sizeof(uint32_t));
	return true;
}

std::string FourccToString(uint32_t fourcc){
	auto data=(char*)&fourcc;
	uint8_t printable=0;
	std::string ret;
	if(fourcc==0)return "(none)";
	for(size_t i=0;i<sizeof(fourcc);i++){
		if(isprint(data[i]))ret+=(char)data[i],printable++;
		else ret+=std::format("\\x{:02X}",data[i]);
	}
	if(printable==0)return std::format("0x{:08X}",fourcc);
	return ret;
}
