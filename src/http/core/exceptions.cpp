/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#define BARE_EXCEPTIONS
#include<cerrno>
#include<cstdlib>
#include<format>
#include"../http.h"
#include"lib/log.h"
#include"lib/exception.h"

Exceptions::HttpError::HttpError(int code,const std::string&msg,std::source_location c){
	this->code=code;
	this->location=c;
	this->original=msg;
	this->msg=std::format(
		"HTTP Error {}: {} at {}:{}",
		code,msg,log::relative_filename(c),c.line()
	);
}
