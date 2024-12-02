/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"posix.h"

void PosixStream::OnStartStream(){
	auto proc=[this](auto e){ProcessEvent(e);};
	if(dir_read||sock_listen)EnablePoll();
	if(sock_listen)for(auto client:clients){
		if(!client.second.poll.IsNull())continue;
		client.second.poll=ctx->loop->AddHandler(
			client.second.fd,EPOLLIN,proc
		);
	}
}

void PosixStream::OnStopStream(){
	if(dir_read||sock_listen)DisablePoll();
	if(sock_listen)for(auto client:clients){
		if(client.second.poll.IsNull())continue;
		ctx->loop->RemoveHandler(client.second.poll);
		client.second.poll.Clear();
	}
}
