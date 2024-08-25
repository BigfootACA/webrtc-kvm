/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"posix.h"
#include"lib/exception.h"

void PosixStream::OnProcessInput(StreamBuffer*buffer){
	if(unlikely(!dir_write))return;
	auto write_plane=[this](int fd,void*ptr,size_t len){
		while(len>0){
			size_t sz=MIN((size_t)mtu,len);
			if(sz<=0)throw InvalidArgument("bad write size");
			auto wr=write(fd,ptr,sz);
			if(wr<0)switch(errno){
				case EINTR:continue;
				case EAGAIN:{
					pollfd fds={.fd=fd,.events=POLLOUT};
					poll(&fds,1,-1);
					continue;
				}
				case EMSGSIZE:if(mode==MODE_SOCKET){
					DetectMTU(fd);
					continue;
				}
				case ECONNREFUSED:case EPIPE:{
					ShutdownFD(fd);
					return;
				}
				default:throw ErrnoException("write failed");
			}else if(wr==0){
				ShutdownFD(fd);
				return;
			}
			ptr=(uint8_t*)ptr+wr,len-=wr;
		}
	};
	auto write_data=[buffer,write_plane](int fd){
		for(uint32_t plane=0;plane<buffer->planes_cnt;plane++){
			auto&p=buffer->planes[plane];
			write_plane(fd,p.ptr,p.used);
		}
	};
	if(!sock_accept)write_data(device_fd);
	else for(auto fd:clients)
		write_data(fd.second.fd);
}
