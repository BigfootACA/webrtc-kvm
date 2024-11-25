/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"
#include"lib/exception.h"

MppBuffer RockchipMediaProcessPlatform::SetupFrameDMABUF(StreamBuffer*buffer){
	MPP_RET ret;
	std::shared_ptr<MppBufferDesc>desc;
	if(unlikely(buffer->planes_cnt!=1))throw InvalidArgument(
		"unsupported planes count {}",buffer->planes_cnt
	);
	auto&p=buffer->planes[0];
	if(p.fd<0)throw InvalidArgument("bad dmabuf fd");
	if(unlikely(!dmabufs.contains(p.fd))){
		desc=std::make_shared<MppBufferDesc>();
		desc->fd=p.fd;
		desc->info.type=MPP_BUFFER_TYPE_DMA_HEAP;
		desc->info.fd=p.fd;
		desc->info.size=p.size;
		desc->info.index=0;
		if(MPP_ISERR(mpp_buffer_import(&desc->buff,&desc->info)))
			throw RuntimeError("mpp import buffer {} failed: {}",p.fd,(int)ret);
		dmabufs[p.fd]=desc;
	}else desc=dmabufs[p.fd];
	return desc->buff;
}

MppBuffer RockchipMediaProcessPlatform::SetupFramePointer(StreamBuffer*buffer){
	if(unlikely(buffer->planes_cnt!=1))throw InvalidArgument(
		"unsupported planes count {}",buffer->planes_cnt
	);
	auto&p=buffer->planes[0];
	void*buf=mpp_buffer_get_ptr(frm_buf);
	size_t sz=mpp_buffer_get_size(frm_buf);
	if(p.used>sz)throw InvalidArgument("buffer too large: {} > {}",p.used,sz);
	memcpy(buf,p.ptr,p.used);
	return frm_buf;
}

void RockchipMediaProcessPlatform::OnProcessInput(StreamBuffer*buffer){
	MppBuffer fp;
	switch(buffer->type){
		case BUFFER_POINTER:fp=SetupFramePointer(buffer);break;
		case BUFFER_DMABUF:fp=SetupFrameDMABUF(buffer);break;
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
