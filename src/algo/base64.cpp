/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"base64.h"

std::string Base64::encode(const void*data,const size_t len){
	auto p=(uint8_t*)data;
	static const char table[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	uint8_t*pos,*end,*in;
	size_t olen=4*((len+2)/3);
	if(olen<len||olen<=0)return "";
	auto out=new uint8_t[olen];
	end=p+len,in=p,pos=out;
	while(end-in>=3){
		*pos++=table[in[0]>>2];
		*pos++=table[((in[0]&0x03)<<4)|(in[1]>>4)];
		*pos++=table[((in[1]&0x0f)<<2)|(in[2]>>6)];
		*pos++=table[in[2]&0x3f];
		in+=3;
	}
	if(end-in){
		*pos++=table[in[0]>>2];
		if(end-in==1){
			*pos++=table[(in[0]&0x03)<<4];
			*pos++='=';
		}else{
			*pos++=table[((in[0]&0x03)<<4)|(in[1]>>4)];
			*pos++=table[(in[1]&0x0f)<<2];
		}
		*pos='=';
	}
	std::string result((char*)out,(char*)out+olen);
	delete[] out;
	return result;
}

std::string Base64::decode(const void*data,const size_t len){
	static const int table[256]={
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,62,63,62,62,63,52,53,54,55,56,57,58,59,60,
		61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
		19,20,21,22,23,24,25,0,0,0,0,63,0,26,27,28,29,30,31,32,33,34,35,
		36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51
	};
	auto p=(uint8_t*)data;
	int pad=len>0&&(len%4||p[len-1]=='=');
	const size_t ilen=((len+3)/4-pad)*4;
	std::string str(ilen/4*3+pad,'\0');
	for(size_t i=0,j=0;i<ilen;i+=4){
		int n=table[p[i]]<<18|table[p[i+1]]<<12|table[p[i+2]]<<6|table[p[i+3]];
		str[j++]=n>>16,str[j++]=n>>8&0xFF,str[j++]=n&0xFF;
	}
	if(pad){
		int n=table[p[ilen]]<<18|table[p[ilen+1]]<<12;
		str[str.size()-1]=n>>16;
		if(len>ilen+2&&p[ilen+2]!='='){
			n|=table[p[ilen+2]]<<6;
			str.push_back(n>>8&0xFF);
		}
	}
	return str;
}
