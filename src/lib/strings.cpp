/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<algorithm>
#include<cstring>
#include<string>
#include<format>
#include"strings.h"

bool CheckValid(const char*val,const char*valids){
	for(size_t i=0;val[i];i++)
		if(!strchr(valids,val[i]))return false;
	return true;
}

std::string ToHexString(uint64_t num,size_t len,bool prefix,bool upper){
	std::string str;
	bool found=false;
	const char*maps=upper?"0123456789ABCDEF":"0123456789abcdef";
	if(prefix)str+="0x";
	ssize_t max=sizeof(num)*2;
	for(ssize_t i=max;i<(ssize_t)len;i++)str+="0";
	for(ssize_t i=max-1;i>=0;i--){
		uint8_t v=num>>(i*4)&0xF;
		if(v==0&&!found&&i>=(ssize_t)len)continue;
		found=true,str+=maps[v];
	}
	return str;
}

void StringReplaceAll(std::string&str,const std::string&from,const std::string&to){
	size_t pos=str.find(from);
	while(pos!=std::string::npos){
		str.replace(pos,from.size(),to);
		pos=str.find(from,pos+to.size());
	}
}

void StringToUpper(std::string&str){
	std::transform(str.begin(),str.end(),str.begin(),::toupper);
}

void StringToLower(std::string&str){
	std::transform(str.begin(),str.end(),str.begin(),::tolower);
}

int StringToNumber(const std::string&str,int def,bool*fail){
	size_t off=0;
	int base=0;
	std::string d=str;
	if(fail)*fail=true;
	if(str.empty())return def;
	if(str.starts_with("0x"))base=16,d=d.substr(2);
	auto ret=std::stoi(d,&off,base);
	if(off!=str.length())return def;
	if(fail)*fail=false;
	return ret;
}

long StringToNumber(const std::string&str,long def,bool*fail){
	size_t off=0;
	int base=0;
	std::string d=str;
	if(fail)*fail=true;
	if(str.empty())return def;
	if(str.starts_with("0x"))base=16,d=d.substr(2);
	auto ret=std::stol(str,&off,base);
	if(off!=str.length())return def;
	if(fail)*fail=false;
	return ret;
}

long long StringToNumber(const std::string&str,long long def,bool*fail){
	size_t off=0;
	int base=0;
	std::string d=str;
	if(fail)*fail=true;
	if(str.empty())return def;
	if(str.starts_with("0x"))base=16,d=d.substr(2);
	auto ret=std::stoll(str,&off,base);
	if(off!=str.length())return def;
	if(fail)*fail=false;
	return ret;
}

unsigned int StringToNumber(const std::string&str,unsigned int def,bool*fail){
	size_t off=0;
	int base=0;
	std::string d=str;
	if(fail)*fail=true;
	if(str.empty())return def;
	if(str.starts_with("0x"))base=16,d=d.substr(2);
	auto ret=std::stoul(str,&off,base);
	if(off!=str.length())return def;
	if(fail)*fail=false;
	return ret;
}

unsigned long StringToNumber(const std::string&str,unsigned long def,bool*fail){
	size_t off=0;
	int base=0;
	std::string d=str;
	if(fail)*fail=true;
	if(str.empty())return def;
	if(str.starts_with("0x"))base=16,d=d.substr(2);
	auto ret=std::stoul(str,&off,base);
	if(off!=str.length())return def;
	if(fail)*fail=false;
	return ret;
}

unsigned long long StringToNumber(const std::string&str,unsigned long long def,bool*fail){
	size_t off=0;
	int base=0;
	std::string d=str;
	if(fail)*fail=true;
	if(str.empty())return def;
	if(str.starts_with("0x"))base=16,d=d.substr(2);
	auto ret=std::stoull(str,&off,base);
	if(off!=str.length())return def;
	if(fail)*fail=false;
	return ret;
}

float StringToNumber(const std::string&str,float def,bool*fail){
	size_t off=0;
	if(fail)*fail=true;
	if(str.empty())return def;
	auto ret=std::stof(str,&off);
	if(off!=str.length())return def;
	if(fail)*fail=false;
	return ret;
}

double StringToNumber(const std::string&str,double def,bool*fail){
	size_t off=0;
	if(fail)*fail=true;
	if(str.empty())return def;
	auto ret=std::stod(str,&off);
	if(off!=str.length())return def;
	if(fail)*fail=false;
	return ret;
}

long double StringToNumber(const std::string&str,long double def,bool*fail){
	size_t off=0;
	if(fail)*fail=true;
	if(str.empty())return def;
	auto ret=std::stold(str,&off);
	if(off!=str.length())return def;
	if(fail)*fail=false;
	return ret;
}

char dec2hex(int dec,bool upper){
	if(dec>=0&&dec<=9)return (char)('0'+dec);
	else if(dec>=10&&dec<=15)return (char)((upper?'A':'a')+dec-10);
	else return 0;
}

int hex2dec(char hex){
	if(hex>='0'&&hex<='9')return hex-'0';
	if(hex>='a'&&hex<='f')return hex-'a'+0xA;
	if(hex>='A'&&hex<='F')return hex-'A'+0xA;
	return 16;
}

bool StringIsTrue(std::string str){
	StringToLower(str);
	if(str=="1")return true;
	if(str=="ok")return true;
	if(str=="on")return true;
	if(str=="yes")return true;
	if(str=="true")return true;
	if(str=="always")return true;
	if(str=="enable")return true;
	if(str=="enabled")return true;
	return false;
}

bool StringIsFalse(std::string str){
	StringToLower(str);
	if(str=="0")return true;
	if(str=="no")return true;
	if(str=="off")return true;
	if(str=="false")return true;
	if(str=="never")return true;
	if(str=="disable")return true;
	if(str=="disabled")return true;
	return false;
}

std::string LinuxVersionToString(uint32_t ver){
	return std::format(
		"{}.{}.{}",
		ver>>0x10&0xff,
		ver>>0x08&0xff,
		ver>>0x00&0xff
	);
}

std::string CharPointerToString(const char*pointer,size_t size){
	if(!pointer||size<=0)return "";
	size_t len=strnlen(pointer,size);
	return std::string(pointer,len);
}

std::string NonEmpty(const std::string&str,const std::string&empty){
	return str.empty()?empty:str;
}
