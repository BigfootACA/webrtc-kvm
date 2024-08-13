/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<cstdlib>
#include"blob.h"
#include"defines.h"
#include"lib/exception.h"

Blob::~Blob(){
	if(buffer&&alloc)free(buffer);
	buffer=nullptr,alloc=false,len=0;
}

Blob::Blob(const std::string&str,bool copy){
	buffer=copy?(uint8_t*)strndup(str.c_str(),str.length()):(uint8_t*)str.c_str();
	alloc=copy,len=str.length();
}

Blob::Blob(size_t length){
	buffer=(uint8_t*)malloc(length);
	if(!buffer)throw ErrnoException("alloc buffer failed");
	alloc=true,len=length;
}

Blob::Blob(void*ptr,size_t len,bool alloc)
	:len(len),alloc(alloc),buffer((uint8_t*)ptr){}


void Blob::Resize(size_t len){
	if(!alloc)throw InvalidArgument("cannot resize a non-allocated blob");
	if(len==this->len)return;
	void*p=realloc(buffer,len);
	if(!p)throw ErrnoException("realloc failed");
	buffer=(uint8_t*)p;
	this->len=len;
}

uint8_t&Blob::operator[](size_t idx){
	if(unlikely(!buffer))throw InvalidArgument("no value");
	if(unlikely(idx>=len))throw InvalidArgument("bad index");
	return buffer[idx];
}

const uint8_t&Blob::operator[](size_t idx)const{
	if(unlikely(!buffer))throw InvalidArgument("no value");
	if(unlikely(idx>=len))throw InvalidArgument("bad index");
	return buffer[idx];
}
