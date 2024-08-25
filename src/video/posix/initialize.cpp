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

void PosixStream::OnInitialize(){
	switch(GetType()){
		case STREAM_SINK:dir_read=false,dir_write=true;break;
		case STREAM_SOURCE:dir_read=true,dir_write=false;break;
		case STREAM_PIPE:dir_read=true,dir_write=true;break;
		default:throw InvalidArgument("unknown stream type");
	}
	Parse();
	log_info("try to intialize {}",url.ToString());
	switch(mode){
		case MODE_SOCKET:OpenSocket();break;
		case MODE_FILE:OpenFile();break;
		case MODE_FD:OpenFD();break;
		default:throw InvalidArgument("unknown mode");
	}
	if(dir_read)for(int i=0;;i++){
		auto&size=OutputPlaneSize(0,i);
		if(size<=0)break;
		DataBuffer buff{.size=size};
		if(!(buff.buffer=malloc(size)))
			throw ErrnoException("allocate {} bytes failed",size);
		buffers[i]=buff;
	}
}

void PosixStream::OnDeinitialize(){
	if(want_close){
		if(sock_accept){
			for(auto client:clients)
				close(client.second.fd);
			clients.clear();
		}
		if(device_fd>=0){
			close(device_fd);
			device_fd=-1;
		}
	}
	for(auto buff:buffers)
		if(buff.buffer)free(buff.buffer);
	buffers.clear();
}
