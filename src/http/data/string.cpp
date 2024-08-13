/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

bool StringToHttpMethod(const std::string&val,HttpMethod&method){
	method=HTTP_METHOD_UNKNOWN;
	#define METHOD(type) else if(val==MHD_HTTP_METHOD_##type)method=HTTP_METHOD_##type;
	if(val.empty())return false;
	#include"methods.h"
	else return false;
	#undef METHOD
	return true;
}

bool StringToHttpVersion(const std::string&val,HttpVersion&version){
	version=HTTP_VERSION_UNKNOWN;
	#define VERSION(name,value) else if(val==#value)version=name;
	if(val.empty())return false;
	#include"versions.h"
	else return false;
	#undef VERSION
	return true;
}

std::string HttpMethodToString(HttpMethod method){
	std::string res;
	#define METHOD(type)\
		if(have_bit(method,HTTP_METHOD_##type)){\
			if(!res.empty())res+=",";\
			res+=MHD_HTTP_METHOD_##type;\
		}
	#include"methods.h"
	#undef METHOD
	return res;
}

std::string HttpVersionToString(HttpVersion method){
	std::string res;
	#define VERSION(name,value)\
		if(have_bit(method,name)){\
			if(!res.empty())res+=",";\
			res+=#value;\
		}
	#include"versions.h"
	#undef VERSION
	return res;
}
