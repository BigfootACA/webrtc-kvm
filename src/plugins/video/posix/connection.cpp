/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"posix.h"
#include"lib/exception.h"
#include"abstract/async-runner.h"

void PosixStream::DetectMTU(int fd){
	int new_mtu=mtu;
	socklen_t xsize=sizeof(new_mtu);
	int r=getsockopt(fd,IPPROTO_IP,IP_MTU,&new_mtu,&xsize);
	if(r!=0)throw ErrnoException("get mtu size failed");
	if(new_mtu>=mtu||new_mtu==0)
		throw InvalidArgument("bad new mtu size: {}",new_mtu);
	mtu=new_mtu;
}

void PosixStream::ShutdownFD(int fd,bool direct){
	auto run=[this,fd](auto){
		if(fd!=device_fd){
			if(clients.contains(fd)){
				auto poll=clients[fd].poll;
				clients.erase(fd);
				ctx->loop->RemoveHandler(poll);
			}
			close(fd);
		}else StopStream();
	};
	if(direct)run(nullptr);
	else AsyncRunner::DefaultRunLater(run);
}

void PosixStream::ProcessNewClient(){
	SocketClient clt{};
	if((clt.fd=accept(device_fd,&clt.addr.sa,&clt.addr.slen))<0)return;
	clients[clt.fd]=clt;
	AsyncRunner::DefaultRunLater([fd=clt.fd,this](auto){
		auto proc=[this](auto e){ProcessEvent(e);};
		clients[fd].poll=ctx->loop->AddHandler(fd,EPOLLIN,proc);
	});
}

void PosixStream::ProcessEvent(EventHandlerContext*ev){
	if(have_bit(ev->event,EPOLLIN)){
		if(sock_accept&&ev->ev->fd==device_fd){
			ProcessNewClient();
		}else if(dir_read){
			ProcessWriteData(ev->ev->fd);
		}
	}
}
