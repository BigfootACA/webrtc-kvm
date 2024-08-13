/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

void HttpHeaders::Add(const std::string&name,const std::string&value){
	headers.push_back({name,value});
}

void HttpHeaders::Replace(const std::string&name,const std::string&value){
	Remove(name);
	Add(name,value);
}

void HttpHeaders::Ensure(const std::string&name,const std::string&value){
	if(!Have(name))Add(name,value);
}

void HttpHeaders::Remove(const std::string&name){
	for(auto it=headers.begin();it!=headers.end();){
		if(it->first==name)headers.erase(it);
		else it++;
	}
}

bool HttpHeaders::Have(const std::string&name)const{
	for(auto h:headers)
		if(h.first==name)
			return true;
	return false;
}

std::vector<std::string>HttpHeaders::Get(const std::string&name)const{
	std::vector<std::string>res;
	for(auto h:headers)
		if(h.first==name)
			res.push_back(h.second);
	return res;
}

std::string HttpHeaders::GetOne(const std::string&name)const{
	for(auto h:headers)
		if(h.first==name)
			return h.second;
	return "";
}

std::vector<std::string>HttpHeaders::GetList(const std::string&name)const{
	return ParseList(Get(name));
}

void HttpHeaders::FillToResponse(MHD_Response*res)const{
	for(auto h:headers)MHD_add_response_header(res,h.first.c_str(),h.second.c_str());
}

void HttpHeaders::FillFromConnection(MHD_Connection*conn){
	MHD_get_connection_values(conn,MHD_HEADER_KIND,[](
		void *cls,
		enum MHD_ValueKind kind,
		const char*key,
		const char*value
	)->MHD_Result {
		auto hdr=(HttpHeaders*)cls;
		hdr->Add(key,value);
		return MHD_YES;
	},this);
}

#define GMT_FORMAT "%a, %d %b %Y %H:%M:%S %Z"

std::string HttpHeaders::TimeToString(time_t t){
	char buf[256];
	memset(buf,0,sizeof(buf));
	auto m=*gmtime(&t);
	strftime(buf,sizeof(buf),GMT_FORMAT,&m);
	return buf;
}

time_t HttpHeaders::StringToTime(const std::string&gmt){
	tm t={};
	auto cs=gmt.c_str();
	auto r=strptime(cs,GMT_FORMAT,&t);
	if(!r||r!=cs+gmt.length())throw HttpError(
		MHD_HTTP_BAD_REQUEST,"bad GMT time"
	);
	return mktime(&t);
}

std::vector<std::string>HttpHeaders::ParseList(const std::string&var){
	std::vector<std::string>v;
	if(var.empty())return v;
	std::string::size_type off=0,i,noff;
	auto len=var.length();
	do{
		i=var.find(',',off);
		if(i!=std::string::npos)noff=i+1;
		else i=len,noff=0;
		while(off<i&&isspace(var[off]))off++;
		while(i>off&&isspace(var[i-1]))i--;
		auto r=var.substr(off,i-off);
		if(!r.empty())v.push_back(r);
		off=noff;
	}while(noff>0);
	return v;
}

std::vector<std::string>HttpHeaders::ParseList(const std::vector<std::string>list){
	std::vector<std::string>v;
	for(auto var:list){
		auto r=ParseList(var);
		v.insert(v.end(),r.begin(),r.end());
	}
	return v;
}
