/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#define BARE_EXCEPTIONS
#include<cerrno>
#include<format>
#include"log.h"
#include"exception.h"

Exceptions::RuntimeErrorImpl::RuntimeErrorImpl(const std::string&msg,std::source_location c){
	this->location=c;
	this->original=msg;
	this->msg=std::format(
		"{} at {}:{}",
		msg,log::relative_filename(c),c.line()
	);
}

Exceptions::InvalidArgumentImpl::InvalidArgumentImpl(const std::string&msg,std::source_location c){
	this->location=c;
	this->original=msg;
	this->msg=std::format(
		"{} at {}:{}",
		msg,log::relative_filename(c),c.line()
	);
}

Exceptions::ErrnoExceptionImpl::ErrnoExceptionImpl(int err,const std::string&msg,std::source_location c){
	this->location=c;
	this->original=msg;
	this->msg=std::format(
		"{} at {}:{}",
		msg,log::relative_filename(c),c.line()
	);
	if(err!=0){
		this->err=err;
		if(!this->msg.empty())this->msg+=": ";
		this->msg+=strerror(err);
	}
}
