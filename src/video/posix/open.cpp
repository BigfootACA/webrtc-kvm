/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"posix.h"
#include"lib/log.h"
#include"lib/exception.h"
#include"lib/strings.h"

void PosixStream::OpenSocket(){
	want_close=true;
	if((addr.slen=url.ToSockAddrIn(&addr.sa,addr.sa.sa_family))==0)
		throw InvalidArgument("parse address failed");
	if((device_fd=::socket(addr.sa.sa_family,sock_type,sock_proto))<0)
		throw ErrnoException("create socket failed");
	if(sock_listen){
		if(::bind(device_fd,&addr.sa,addr.slen)!=0)
			throw ErrnoException("bind socket failed");
		if(sock_type==SOCK_STREAM&&::listen(device_fd,1)!=0)
			throw ErrnoException("listen socket failed");
	}else if(::connect(device_fd,&addr.sa,addr.slen)!=0)
		throw ErrnoException("connect socket failed");
}

void PosixStream::OpenFile(){
	want_close=true;
	auto path=url.GetPath();
	if(path.empty())throw InvalidArgument("no path set for open file");
	int flags=O_CLOEXEC|O_NONBLOCK|O_TRUNC|O_CREAT;
	if(dir_write&&dir_read)flags|=O_RDWR;
	else if(dir_write)flags|=O_WRONLY;
	else if(dir_read)flags|=O_RDONLY;
	if((device_fd=open(path.c_str(),flags,0644))<0)
		throw ErrnoException("open {} failed",path);
	log_info("opened {} as fd {}",path,device_fd);
}

void PosixStream::OpenFD(){
	want_close=false;
	if((device_fd=StringToNumber(url.GetHost(),(int)-1))<0)
		throw InvalidArgument("bad fd for stream");
	log_info("use fd {} for {}",device_fd,GetID());
}

void PosixStream::Parse(){
	auto sch=url.GetScheme();
	if(sch.empty())throw InvalidArgument("no scheme specified");
	mode=MODE_UNKNOWN;
	bool udp=sch.starts_with("udp");
	bool tcp=sch.starts_with("tcp");
	if(udp||tcp){
		mode=MODE_SOCKET;
		if(udp==tcp)throw InvalidArgument("unexcepted protocol found");
		if(udp)sock_type=SOCK_DGRAM,sock_proto=IPPROTO_UDP;
		if(tcp)sock_type=SOCK_STREAM,sock_proto=IPPROTO_TCP;
		if(sch.size()>4){
			auto p=sch.substr(3,1);
			if(p=="4")addr.sa.sa_family=AF_INET;
			if(p=="6")addr.sa.sa_family=AF_INET6;
		}
		sock_accept=tcp;
		sock_listen=
			sch.ends_with("-srv")||
			sch.ends_with("-server")||
			sch.ends_with("-listen");
	}
	if(sch=="file")mode=MODE_FILE;
	if(sch=="fd")mode=MODE_FD;
	if(mode==MODE_FD)throw InvalidArgument("unsupported scheme {}",sch);
}
