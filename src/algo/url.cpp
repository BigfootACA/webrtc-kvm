/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"url.h"
#include"lib/strings.h"
#include"lib/exception.h"

void URL::Parse(const char*u,size_t len){
	size_t s;
	char*c,*p=(char*)u;
	if(!u||!*u)return;
	if(len<=0)len=strlen(u);
	while(len>0&&isspace(u[0]))u++,len--;
	while(len>0&&isspace(u[len-1]))len--;
	if(len<=0)return;
	Clean();
	if((c=strstr(p,"://"))){
		if((s=c-p)>0)SetScheme(p,s);
		p=c+3;
	}
	if((c=strpbrk(p,"@/"))&&*c=='@'){
		char*info=p;
		size_t si=c-p;
		p=c+1;
		if((c=(char*)memchr(info,':',si))){
			s=c-info;
			SetUsername(info,s);
			SetPassword(c+1,p-c-2);
		}else SetUsername(info,si);
	}
	if(*p=='['){
		if(!(c=strchr(p,']')))
			throw InvalidArgument("missing ']'");
		SetHost(p+1,c-p-1);
		p=c+1;
	}else if(*p!='/'&&*p!=':'&&*p!='?'&&*p!='#'){
		c=strpbrk(p,":/?#"),s=c?(size_t)(c-p):0;
		SetHost(p,s);
		if(!c)return;
		p=c;
	}
	if(*p==':'){
		p++,c=strpbrk(p,"/?#"),s=c?(size_t)(c-p):0;
		SetPort(p,s);
		if(!c)return;
	}
	if(*p=='/'){
		c=strpbrk(p,"?#");
		SetPath(p,c?c-p:0);
		if(!c)return;
		p=c;
	}
	if(*p=='?'){
		p++,c=strchr(p,'#');
		SetQuery(p,c?c-p:0);
		if(!c)return;
		p=c;
	}
	if(*p=='#')SetFragment(p+1,0);
}

void URL::Clean(){
	SetScheme("");
	SetUsernameDecoded("");
	SetPasswordDecoded("");
	SetHostDecoded("");
	SetPort(-1);
	SetPathDecoded("");
	SetQuery("");
	SetFragment("");
}

void URL::From(const URL*url){
	if(!url)return;
	this->SetScheme(url->GetScheme());
	this->SetUsernameDecoded(url->GetUsername());
	this->SetPasswordDecoded(url->GetPassword());
	this->SetHostDecoded(url->GetHost());
	this->SetPort(url->GetPort());
	this->SetPathDecoded(url->GetPath());
	this->SetQuery(url->GetQuery());
	this->SetFragment(url->GetFragment());
}

int URL::Compare(const std::string&u)const{
	URL*url;
	try{url=new URL(u);}catch(...){return -1;}
	return this->Compare(url);
}

int URL::Compare(const URL*that)const{
	try{
		auto l=this->ToString();
		auto r=that->ToString();
		return l.compare(r);
	}catch(...){
		return -1;
	}
}

void URL::SetPort(const std::string&val){
	bool fail=false;
	auto r=StringToNumber(val,(int)-1,&fail);
	if(fail)throw InvalidArgument("bad port");
	SetPort(r);
}

bool URL::GoBack(){
	if(IsInTop())return false;
	size_t len=path.length();
	if(path.ends_with("/"))path.pop_back();
	while(!path.ends_with("/")&&!path.empty())
		path.pop_back();
	return path.length()!=len;
}

size_t URL::AppendAll(std::string&val)const{
	size_t len=val.length();
	AppendScheme(val);
	AppendHierarchical(val);
	AppendQuery(val);
	AppendFragment(val);
	return val.length()-len;
}

size_t URL::AppendOrigin(std::string&val)const{
	size_t len=val.length();
	AppendHost(val);
	AppendPort(val);
	return val.length()-len;
}

size_t URL::AppendHierarchical(std::string&val)const{
	size_t len=val.length();
	AppendAuthority(val);
	AppendPath(val);
	return val.length()-len;
}

size_t URL::AppendUserInfo(std::string&val)const{
	size_t len=val.length();
	AppendUsername(val);
	AppendPassword(val);
	return val.length()-len;
}

size_t URL::AppendAuthority(std::string&val)const{
	size_t len=val.length();
	auto s=AppendUserInfo(val);
	if(s>0)val+="@";
	AppendOrigin(val);
	return val.length()-len;
}

size_t URL::AppendFullPath(std::string&val)const{
	size_t len=val.length();
	AppendPath(val);
	AppendQuery(val);
	AppendFragment(val);
	return val.length()-len;
}

size_t URL::AppendScheme(std::string&val)const{
	size_t len=val.length();
	if(!scheme.empty())val+=scheme+":";
	val+="//";
	return val.length()-len;
}

size_t URL::AppendUsername(std::string&val)const{
	size_t len=val.length();
	if(!username.empty())val+=Encode(username);
	return val.length()-len;
}

size_t URL::AppendPassword(std::string&val)const{
	size_t len=val.length();
	if(!password.empty())val+=":"+Encode(password);
	return val.length()-len;
}

size_t URL::AppendHost(std::string&val)const{
	size_t len=val.length();
	if(strpbrk(host.c_str(),"[]#?%"))
		val+=Encode(host);
	else if(strpbrk(host.c_str(),":/@"))
		val+=std::format("[{0}]",host);
	else val+=host;
	return val.length()-len;
}

size_t URL::AppendPort(std::string&val)const{
	size_t len=val.length();
	if(port>=0)val+=std::format(":{0:d}",port);
	return val.length()-len;
}

size_t URL::AppendPath(std::string&val)const{
	size_t len=val.length();
	if(!path.starts_with("/"))val+="/";
	val+=URL::Encode(path,nullptr,"/");
	return val.length()-len;
}

size_t URL::AppendQuery(std::string&val)const{
	size_t len=val.length();
	if(!query.empty()){
		if(!val.empty())val+="?";
		val+=query;
	}
	return val.length()-len;
}

size_t URL::AppendFragment(std::string&val)const{
	size_t len=val.length();
	if(!fragment.empty()){
		if(!val.empty())val+="#";
		val+=fragment;
	}
	return val.length()-len;
}

static char url_encoding_map[256];
static bool url_encoding_maps_initialized=false;

static void init_url_encoding_map(){
	unsigned char i;
	if(url_encoding_maps_initialized)return;
	memset(url_encoding_map,0,sizeof(url_encoding_map));
	for(i='a';i<='z';i++)url_encoding_map[i]=(char)i;
	for(i='A';i<='Z';i++)url_encoding_map[i]=(char)i;
	for(i='0';i<='9';i++)url_encoding_map[i]=(char)i;
	url_encoding_map[(uint32_t)'-']='-';
	url_encoding_map[(uint32_t)'_']='_';
	url_encoding_map[(uint32_t)'.']='.';
	url_encoding_map[(uint32_t)'*']='*';
	url_encoding_maps_initialized=true;
}

std::string URL::Encode(const std::string&src,const char*map,const std::string&skip){
	char m[256];
	if(src.empty())return "";
	if(!map){
		init_url_encoding_map();
		memcpy(m,url_encoding_map,sizeof(m));
	}else memcpy(m,map,sizeof(m));
	std::string out;
	for(char c:skip)m[(uint8_t)c]=c;
	for(char c:src){
		if(!m[(uint8_t)c])out+=m[(uint8_t)c];
		else out+=std::format("%{0:2X}",c&0xFF);
	}
	return out;
}

std::string URL::Decode(const std::string&src){
	std::string out;
	size_t len=src.length();
	for(size_t i=0;i<len;i++)switch(src[i]){
		case '%':{
			size_t idx=0;
			if(len-i<3)throw InvalidArgument("bad code");
			auto r=stoi(src.substr(i+1,2),&idx,16);
			if(idx!=2)throw InvalidArgument("bad code hex");
			out+=(char)r;
		}break;
		case '+':out+=' ';break;
		default:out+=src[i];
	}
	return out;
}
