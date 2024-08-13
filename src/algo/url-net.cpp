/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<netdb.h>
#include<arpa/inet.h>
#include"url.h"
#include"defines.h"
#include"lib/exception.h"

enum ip_ver{
	IP_V4   = 1,
	IP_V6   = 2,
	IP_DUAL = IP_V4|IP_V6,
};

static size_t InitAddress(sockaddr*sock,const URL&url,void*addr,size_t len,int af){
	sock->sa_family=af;
	int port=url.GetPort();
	if(port<0)port=0;
	switch(af){
		case AF_INET:{
			auto a=(sockaddr_in*)sock;
			a->sin_port=htons(port);
			memcpy(&a->sin_addr,addr,len);
			return sizeof(sockaddr_in);
		}
		case AF_INET6:{
			auto a=(sockaddr_in6*)sock;
			a->sin6_port=htons(port);
			memcpy(&a->sin6_addr,addr,len);
			return sizeof(sockaddr_in6);
		}
		default:return 0;
	}
}

size_t URL::ToSockAddrIn(sockaddr*addr,int prefer)const{
	char buff[64];
	ip_ver ver;
	if(!addr)throw InvalidArgument("bad sockaddr");
	memset(addr,0,sizeof(sockaddr_storage));
	switch(prefer){
		case 0:ver=IP_DUAL;break;
		case AF_INET:ver=IP_V4;break;
		case AF_INET6:ver=IP_V6;break;
		default:throw InvalidArgument("unsupported prefer family");
	}
	if(host.empty())throw InvalidArgument("host not set");
	hostent*he;
	if(have_bit(ver,IP_V6)&&inet_pton(AF_INET6,host.c_str(),buff)>0)
		return InitAddress(addr,*this,buff,sizeof(in6_addr),AF_INET6);
	if(have_bit(ver,IP_V4)&&inet_pton(AF_INET,host.c_str(),buff)>0)
		return InitAddress(addr,*this,buff,sizeof(in_addr),AF_INET);
	if(have_bit(ver,IP_V6)&&(he=gethostbyname2(host.c_str(),AF_INET6)))
		return InitAddress(addr,*this,he->h_addr,he->h_length,he->h_addrtype);
	if(have_bit(ver,IP_V4)&&(he=gethostbyname2(host.c_str(),AF_INET)))
		return InitAddress(addr,*this,he->h_addr,he->h_length,he->h_addrtype);
	throw InvalidArgument("parse host address failed");
}
