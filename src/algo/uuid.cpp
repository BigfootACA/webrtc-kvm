/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<string>
#include"lib/exception.h"
#include"uuid.h"

void UUID::Generate(){
	uuid_generate(this->uuid);
}

void UUID::GenerateTime(){
	uuid_generate_time(this->uuid);
}

void UUID::GenerateTimeSafe(){
	uuid_generate_time_safe(this->uuid);
}

void UUID::GenerateRandom(){
	uuid_generate_random(this->uuid);
}

void UUID::Set(const void*u){
	if(!u)Clear();
	uuid_copy(this->uuid,(const uint8_t*)u);
}

int UUID::GetType()const{
	return uuid_type(this->uuid);
}

int UUID::GetVariant()const{
	return uuid_variant(this->uuid);
}

void UUID::Clear(){
	uuid_clear(this->uuid);
}

int UUID::Compare(const void*u)const{
	return uuid_compare(this->uuid,(const uint8_t*)u);
}

bool UUID::IsNull()const{
	return !!uuid_is_null(this->uuid);
}

UUID UUID::GetTemplate(const std::string&name){
	return UUID(uuid_get_template(name.c_str()));
}

void UUID::Set(const char*u,size_t len){
	if(!u&&len==0){
		this->Clear();
		return;
	}
	if(!u||len!=36)
		throw RuntimeError("bad uuid");
	if(uuid_parse_range(u,u+len,this->uuid)!=0)
		throw RuntimeError("bad uuid");
}

void UUID::GenerateMD5(const UUID&ns,const std::string&name){
	uuid_generate_md5(
		this->uuid,ns.uuid,
		name.c_str(),name.length()
	);
}
void UUID::GenerateSHA1(const UUID&ns,const std::string&name){
	uuid_generate_sha1(
		this->uuid,ns.uuid,
		name.c_str(),name.length()
	);
}

std::string UUID::ToString()const{
	char buff[UUID_STR_LEN];
	memset(buff,0,sizeof(buff));
	uuid_unparse(this->uuid,buff);
	return buff;
}

std::string UUID::ToUpperString()const{
	char buff[UUID_STR_LEN];
	memset(buff,0,sizeof(buff));
	uuid_unparse_upper(this->uuid,buff);
	return buff;
}

std::string UUID::ToLowerString()const{
	char buff[UUID_STR_LEN];
	memset(buff,0,sizeof(buff));
	uuid_unparse_lower(this->uuid,buff);
	return buff;
}
