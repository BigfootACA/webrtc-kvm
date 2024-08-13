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

Exceptions::RuntimeError::RuntimeError(const std::string&msg,std::source_location c){
	this->location=c;
	this->original=msg;
	this->msg=std::format(
		"{} at {}:{}",
		msg,log::relative_filename(c),c.line()
	);
}

Exceptions::InvalidArgument::InvalidArgument(const std::string&msg,std::source_location c){
	this->location=c;
	this->original=msg;
	this->msg=std::format(
		"{} at {}:{}",
		msg,log::relative_filename(c),c.line()
	);
}

Exceptions::ErrnoException::ErrnoException(const std::string&msg,std::source_location c){
	this->location=c;
	this->original=msg;
	this->msg=std::format(
		"{} at {}:{}",
		msg,log::relative_filename(c),c.line()
	);
	if(errno!=0){
		err=errno;
		if(!this->msg.empty())this->msg+=": ";
		this->msg+=strerror(errno);
	}
}
