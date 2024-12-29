/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<cerrno>
#include<fcntl.h>
#include<cstddef>
#include<unistd.h>
#include<cstdint>
#include<cstdlib>
#include<poll.h>
#include"full_io.h"
#include"defines.h"

ssize_t full_write(int fd,const void*data,size_t len){
	ssize_t sent;
	size_t txl=len;
	const uint8_t*txd=(uint8_t*)data;
	pollfd fds{};
	do{
		errno=0;
		sent=write(fd,txd,txl);
		if(likely(sent>0))txl-=sent,txd+=sent;
		if(likely(sent==0))return (ssize_t)(len-txl);
		if(unlikely(sent<0))switch(errno){
			case EAGAIN:
				fds.revents=0;
				fds.events=POLLOUT;
				fds.fd=fd;
				poll(&fds,1,-1);
			break;
			case EINTR:continue;
			default:return sent;
		}
	}while(txl>0);
	return (ssize_t)len;
}

ssize_t full_read(int fd,void*data,size_t len){
	ssize_t received;
	size_t rxl=len;
	auto rxd=(uint8_t*)data;
	pollfd fds{};
	do{
		errno=0;
		received=read(fd,rxd,rxl);
		if(likely(received>0))rxl-=received,rxd+=received;
		if(likely(received==0))return (ssize_t)(len-rxl);
		if(unlikely(received<0))switch(errno){
			case EAGAIN:
				fds.revents=0;
				fds.events=POLLIN;
				fds.fd=fd;
				poll(&fds,1,-1);
			break;
			case EINTR:continue;
			default:return received;
		}
	}while(rxl>0);
	return (ssize_t)len;
}

ssize_t full_write_file(const char*path,const void*data,size_t len){
	int fd;
	if((fd=open(path,O_TRUNC|O_WRONLY|O_CREAT|O_CLOEXEC,0644))<0)return -errno;
	ssize_t ret=full_write(fd,data,len);
	close(fd);
	return ret;
}

ssize_t full_read_file(const char*path,void*data,size_t len){
	int fd;
	if((fd=open(path,O_RDONLY|O_CLOEXEC))<0)return -errno;
	ssize_t ret=full_read(fd,data,len);
	close(fd);
	return ret;
}

ssize_t full_read_file_alloc(const char*path,void**data){
	int fd;
	void*ptr=nullptr,*x;
	size_t len=0,size=0;
        ssize_t r=0;
	if(!data)return set_errno(EINVAL);
	*data=nullptr;
	if((fd=open(path,O_RDONLY|O_CLOEXEC))<0)return -errno;
	while(true){
		if(!ptr||size-len<0x1000){
			size+=0x1000;
			if((x=ptr?malloc(size):realloc(ptr,size)))ptr=x;
			else goto_errno(ENOMEM);
		}
                while(size-len-1>1){
                        r=full_read(fd,(uint8_t*)ptr+len,size-len-1);
                        if(r<0)goto error;
                        if(r==0)goto end;
                        len+=r;
                }
	}
end:
	((char*)ptr)[len]=0;
	*data=ptr;
	close(fd);
	return (ssize_t)len;
error:
	if(ptr)free(ptr);
	close(fd);
	return -errno;
}
