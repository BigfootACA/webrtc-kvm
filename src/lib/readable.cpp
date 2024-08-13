/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 * Size readable string generate
 *
 */

#include<cstdint>
#include<cstdlib>
#include<cstddef>
#include<cstring>
#include<cstdio>
#include<cinttypes>
#include<format>
#include"readable.h"
#include"defines.h"

const char*size_units_b[]={"B","KB","MB","GB","TB","PB","EB","ZB","YB",NULL};
const char*size_units_ib[]={"B","KiB","MiB","GiB","TiB","PiB","EiB","ZiB","YiB",NULL};
const char*size_units_ibs[]={"B/s","KiB/s","MiB/s","GiB/s","TiB/s","PiB/s","EiB/s","ZiB/s","YiB/s",NULL};
const char*size_units_hz[]={"Hz","KHz","MHz","GHz","THz","PHz","EHz","ZHz","YHz",NULL};

const char*format_size_ex(
	char*buf,size_t len,
	uint64_t val,const char**units,
	size_t blk
){
	int unit=0;
	if(unlikely(!buf||len<=0||!units))return NULL;
	memset(buf,0,len);
	if(val==0)return strncpy(buf,"0",len-1);
	while((val>=blk)&&units[unit+1])val/=blk,unit++;
	snprintf(buf,len-1,"%" PRIu64 " %s",val,units[unit]);
	return buf;
}

const char*format_size_float_ex(
	char*buf,size_t len,
	uint64_t val,const char**units,
	size_t blk,uint8_t dot
){
	int unit=0;
	uint64_t left,right,pd=10;
	if(unlikely(!buf||len<=0||!units))return NULL;
	if(dot==0)return format_size_ex(buf,len,val,units,blk);
	for(;dot>0;dot--)pd*=10;
	memset(buf,0,len);
	if(val==0){
		strncpy(buf,"0",len);
		return buf;
	}
	while((val>=blk*blk)&&units[unit+1])val/=blk,unit++;
	left=val,right=0;
	if(val>=blk&&units[unit+1])left=val/blk,right=(val%blk)*pd/blk,unit++;
	if(right%10>=5)right+=10;
	right/=10;
	while(right>0&&(right%10)==0)right/=10;
	snprintf(buf,len,"%" PRIu64 ".%" PRIu64 " %s",left,right,units[unit]);
	return buf;
}

std::string FormatSize(uint64_t val,const char**units,size_t blk){
	char buff[256];
	if(!format_size_ex(buff,sizeof(buff),val,units,blk))return "";
	return buff;
}

std::string FormatSizeFloat(uint64_t val,const char**units,size_t blk,uint8_t dot){
	char buff[256];
	if(!format_size_float_ex(buff,sizeof(buff),val,units,blk,dot))return "";
	return buff;
}
std::string FormatSizeHz(uint64_t val){
	return FormatSize(val,size_units_hz,1000);
}

std::string FormatSizeFloatHz(uint64_t val,uint8_t dot){
	return FormatSizeFloat(val,size_units_hz,1000,dot);
}

std::string SizeString(uint64_t val){
	return std::format("{} ({} bytes)",FormatSize(val),val);
}

std::string SizeStringFloat(uint64_t val){
	return std::format("{} ({} bytes)",FormatSizeFloat(val),val);
}
