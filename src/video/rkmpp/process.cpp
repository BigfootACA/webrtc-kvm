/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"
#include"lib/exception.h"

MppBuffer RockchipMediaProcessPlatform::SetupFrameDMABUF(int fd,size_t used,size_t size){
	MPP_RET ret;
	std::shared_ptr<MppBufferDesc>desc;
	if(fd<0)throw InvalidArgument("bad dmabuf fd");
	if(unlikely(!dmabufs.contains(fd))){
		desc=std::make_shared<MppBufferDesc>();
		desc->fd=fd;
		desc->info.type=MPP_BUFFER_TYPE_DMA_HEAP;
		desc->info.fd=fd;
		desc->info.size=size;
		desc->info.index=0;
		if(MPP_ISERR(mpp_buffer_import(&desc->buff,&desc->info)))
			throw RuntimeError("mpp import buffer {} failed: {}",fd,(int)ret);
		dmabufs[fd]=desc;
	}else desc=dmabufs[fd];
	return desc->buff;
}

MppBuffer RockchipMediaProcessPlatform::SetupFramePointer(void*ptr,size_t used,size_t size){
	void*buf=mpp_buffer_get_ptr(frm_buf);
	size_t sz=mpp_buffer_get_size(frm_buf);
	if(used>sz)throw InvalidArgument("buffer too large: {} > {}",used,sz);
	memcpy(buf,ptr,used);
	return frm_buf;
}

void RockchipMediaProcessPlatform::OnProcessInput(StreamBuffer*buffer){
	if(unlikely(buffer->planes_cnt!=1))throw InvalidArgument(
		"unsupported planes count {}",buffer->planes_cnt
	);
	MppBuffer fp;
	auto&p=buffer->planes[0];
	switch(buffer->type){
		case BUFFER_POINTER:fp=SetupFramePointer(p.ptr,p.used,p.size);break;
		case BUFFER_DMABUF:fp=SetupFrameDMABUF(p.fd,p.used,p.size);break;
		default:throw InvalidArgument("unsupported buffer type");
	}
	if(unlikely(!fp))throw InvalidArgument("no buffer got");
	switch(role){
		case ROLE_ENCODER:OnProcessInputEncoder(fp,buffer);break;
		case ROLE_DECODER:OnProcessInputDecoder(fp,buffer);break;
		default:throw InvalidArgument("unsupported rkmpp role");
	}
	frames++;
}
