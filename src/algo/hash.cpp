/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"hash.h"
#include"lib/exception.h"
#include<openssl/err.h>
#include<openssl/rand.h>

std::string Hash::BinaryToHexStringByTable(const char*table,const void*data,size_t len){
	if(len>=0x10000)throw InvalidArgument("data too large");
	auto src=(const uint8_t*)data;
	char*buffer=new char[len*2+1];
	for(size_t i=0;i<len;i++){
		buffer[i*2+0]=table[(src[i]>>4)&0xF];
		buffer[i*2+1]=table[(src[i]>>0)&0xF];
	}
	buffer[len*2]=0;
	std::string ret(buffer);
	delete[] buffer;
	return ret;
}

static constexpr const int16_t hex_map[]={
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

void Hash::HexStringToBinary(const char*str,size_t len,void*data){
	auto dest=(uint8_t*)data;
	memset(dest,0,len/2);
	for(size_t i=0;i<len;i++){
		int16_t val=hex_map[(int)(uint8_t)str[i]];
		if(val<0)throw InvalidArgument("bad hex string");
		dest[i/2]|=val<<(4*!(i%2));
	}
}

void Hash::RandomBytes(void*buffer,size_t len){
	char error[256];
	if(len>INT_MAX)throw InvalidArgument("random size to large");
	int r=RAND_bytes((uint8_t*)buffer,(int)len);
	if(r!=1)throw RuntimeError(
		"generate random bytes failed: {}",
		ERR_error_string(ERR_get_error(),error)
	);
}

std::string Hash::RandomUpperHexString(size_t len){
	auto buffer=new uint8_t[len];
	RandomBytes(buffer,len);
	auto ret=BinaryToUpperHexString(buffer,len);
	delete[] buffer;
	return ret;
}

std::string Hash::RandomLowerHexString(size_t len){
	auto buffer=new uint8_t[len];
	RandomBytes(buffer,len);
	auto ret=BinaryToLowerHexString(buffer,len);
	delete[] buffer;
	return ret;
}

std::string Hash::ToHexStringByTable(const char*table){
	return BinaryToHexStringByTable(table,ToBinary(),GetHashBytes());
}

std::string Hash::BinaryToUpperHexString(const void*data,size_t len){
	return BinaryToHexStringByTable(upper_table,data,len);
}

std::string Hash::BinaryToLowerHexString(const void*data,size_t len){
	return BinaryToHexStringByTable(lower_table,data,len);
}

void Hash::HexStringToBinary(const char*str,void*data){
	HexStringToBinary(str,strlen(str),data);
}

void Hash::HexStringToBinary(const std::string&str,void*data){
	HexStringToBinary(str.c_str(),str.length(),data);
}

void*Hash::HexStringToBinary(const char*str,size_t len){
	auto buffer=new uint8_t[len/2];
	HexStringToBinary(str,len,buffer);
	return buffer;
}

void*Hash::HexStringToBinary(const char*str){
	return HexStringToBinary(str,strlen(str));
}

void*Hash::HexStringToBinary(const std::string&str){
	return HexStringToBinary(str.c_str(),str.length());
}
