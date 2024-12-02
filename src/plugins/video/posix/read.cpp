/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"posix.h"
#include"lib/exception.h"

void PosixStream::ProcessWriteData(int fd){
	auto idx=clients.find(fd);
	if(idx==clients.end())return;
	bool full=true;
	if(buffers.empty())throw InvalidArgument("no buffers for write data");
	for(size_t i=0;i<buffers.size();i++){
		auto&buff=buffers[i];
		auto ptr=buff.buffer;
		auto remain=buff.size-buff.offset;
		while(remain>0){
			auto rd=read(device_fd,ptr,remain);
			if(rd<0)switch(errno){
				case EAGAIN:return;
				case EINTR:continue;
				case ECONNREFUSED:case EPIPE:{
					ShutdownFD(fd);
					return;
				}
				default:throw ErrnoException("read failed");
			}else if(rd==0){
				ShutdownFD(fd);
				return;
			}
			if((size_t)rd>remain)throw InvalidArgument("unexcepted read size");
			ptr=(uint8_t*)ptr+rd,remain-=rd,buff.offset+=rd;
		}
		if(buff.size!=buff.offset)full=false;
	}
	if(!full)return;
	StreamBuffer buff{.type=BUFFER_POINTER};
	buff.planes_cnt=buffers.size();
	gettimeofday(&buff.timestamp,nullptr);
	for(size_t i=0;i<buffers.size();i++){
		auto&b=buffers[i];
		auto&p=buff.planes[i];
		p.ptr=b.buffer;
		p.size=b.size;
		p.used=b.offset;
		b.offset=0;
	}
	SendToNext(&buff);
}
