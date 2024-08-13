/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"hash.h"
#include"crc32.h"
#include"lib/exception.h"
#include<string>
#include<openssl/evp.h>

class CRC32:public Hash{
	public:
		inline Hash*Init()override{val=0;return this;}
		inline Hash*Reset()override{val=0;return this;}
		inline std::string GetName()override{return "crc32";}
		Hash*Update(const void*data,size_t len)override;
		uint8_t*ToBinary()override{return (uint8_t*)&val;}
		inline size_t GetHashBytes()override{return 4;}
	private:
		uint32_t val=0;
};

class OpenSSLHash:public Hash{
	public:
		virtual const EVP_MD*GetMD()=0;
		Hash*Init()override;
		Hash*Reset()override;
		Hash*Update(const void*data,size_t len)override;
		uint8_t*ToBinary()override;
		size_t GetHashBytes()override;
		OpenSSLHash();
		~OpenSSLHash()override;
	private:
		uint8_t*bin=nullptr;
		EVP_MD_CTX*ctx=nullptr;
		bool final=false;
};

#define DEF_OPENSSL_HASH(_name,_evp) class _name:public OpenSSLHash{\
	public:\
		inline std::string GetName()override{return #_evp;};\
		inline const EVP_MD*GetMD()override{return EVP_##_evp();};\
	}
DEF_OPENSSL_HASH(SM3,sm3);
DEF_OPENSSL_HASH(MD5,md5);
DEF_OPENSSL_HASH(SHA1,sha1);
DEF_OPENSSL_HASH(SHA224,sha224);
DEF_OPENSSL_HASH(SHA256,sha256);
DEF_OPENSSL_HASH(SHA384,sha384);
DEF_OPENSSL_HASH(SHA512,sha512);

OpenSSLHash::OpenSSLHash(){
	if(!(ctx=EVP_MD_CTX_new()))
		throw RuntimeError("alloc message digest context failed");
}

OpenSSLHash::~OpenSSLHash(){
	delete[]bin;
	if(ctx)EVP_MD_CTX_free(ctx);
}

Hash*OpenSSLHash::Init(){
	auto md=GetMD();
	if(!md)throw RuntimeError("bad message digest method");
	EVP_DigestInit(ctx,md);
	delete[]bin;
	bin=new uint8_t[GetHashBytes()];
	memset(bin,0,GetHashBytes());
	return this;
}

Hash*OpenSSLHash::Reset(){
	EVP_MD_CTX_reset(ctx);
	EVP_DigestInit(ctx,GetMD());
	memset(bin,0,GetHashBytes());
	final=false;
	return this;
}

Hash*OpenSSLHash::Update(const void*data,size_t len){
	if(len>0&&data)EVP_DigestUpdate(ctx,data,len);
	return this;
}

uint8_t*OpenSSLHash::ToBinary(){
	if(!final)EVP_DigestFinal(ctx,bin,nullptr);
	final=true;
	return bin;
}

size_t OpenSSLHash::GetHashBytes(){
	return EVP_MD_CTX_get_size(ctx);
}

Hash*CRC32::Update(const void*data,size_t len){
	if(len>0&&data)val=crc32((uint32_t)val,(const uint8_t*)data,len);
	return this;
}

std::unique_ptr<Hash>Hash::NewHash(const std::string&type,const void*data,size_t len){
	std::unique_ptr<Hash>h;
	if(type=="sm3")h=std::make_unique<SM3>();
	else if(type=="md5")h=std::make_unique<MD5>();
	else if(type=="sha1")h=std::make_unique<SHA1>();
	else if(type=="sha224")h=std::make_unique<SHA224>();
	else if(type=="sha256")h=std::make_unique<SHA256>();
	else if(type=="sha384")h=std::make_unique<SHA384>();
	else if(type=="sha512")h=std::make_unique<SHA512>();
	else if(type=="crc32")h=std::make_unique<CRC32>();
	else return nullptr;
	h->Init();
	if(data&&len>0)h->Update(data,len);
	return h;
}
