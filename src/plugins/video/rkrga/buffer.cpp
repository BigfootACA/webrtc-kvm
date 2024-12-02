/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkrga.h"
#include"lib/log.h"
#include"lib/readable.h"
#include"lib/exception.h"
#include"abstract/resource_release.h"

struct dma_heap_allocation_data{
        uint64_t len;
        uint32_t fd;
        uint32_t fd_flags;
        uint64_t heap_flags;
};

struct dma_buf_sync{
	uint64_t flags;
};

#define DMA_HEAP_IOCTL_ALLOC    _IOWR('H', 0, struct dma_heap_allocation_data)
#define DMA_BUF_IOCTL_SYNC      _IOW('b', 0, struct dma_buf_sync)
#define DMA_BUF_SYNC_READ      (1 << 0)
#define DMA_BUF_SYNC_WRITE     (2 << 0)
#define DMA_BUF_SYNC_RW        (DMA_BUF_SYNC_READ | DMA_BUF_SYNC_WRITE)
#define DMA_BUF_SYNC_START     (0 << 2)
#define DMA_BUF_SYNC_END       (1 << 2)

void RGABuffer::Import(StreamBuffer*sb){
	Destroy();
	if(unlikely(desc.Empty()))throw InvalidArgument("empty image desc");
	if(unlikely(desc.size==0))throw InvalidArgument("no size set");
	if(unlikely(sb->planes_cnt!=1))throw InvalidArgument(
		"unsupported planes count {}",sb->planes_cnt
	);
	auto&p=sb->planes[0];
	switch(sb->type){
		case BUFFER_DMABUF:
			handle=importbuffer_fd(p.fd,p.size);
		break;
		case BUFFER_POINTER:
			handle=importbuffer_virtualaddr(p.ptr,p.size);
		break;
		default:throw InvalidArgument("bad buffer type");
	}
	if(unlikely(!handle))throw RuntimeError("import source buffer failed");
	buffer=wrapbuffer_handle(handle,desc.width,desc.height,desc.format);
}

void RGABuffer::Create(int dmabuf_dev_fd){
	Destroy();
	if(unlikely(desc.Empty()))throw InvalidArgument("empty image desc");
	if(unlikely(desc.size==0))throw InvalidArgument("no size set");
	dma_heap_allocation_data data{
		.len=desc.size,
		.fd_flags=O_CLOEXEC|O_RDWR,
	};
	xioctl(dmabuf_dev_fd,DMA_HEAP_IOCTL_ALLOC,&data);
	allocated=true;
	FDRelease dmabuf_buf_f(data.fd);
	pointer=(void*)mmap(
		nullptr,data.len,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,data.fd,0
	);
	if(!pointer||pointer==MAP_FAILED)throw RuntimeError(
		"map dmabuf failed: {}",strerror(errno)
	);
	log_info(
		"dmabuf allocate buffer {} size {} fd {} at 0x{:X}",
		index,SizeStringFloat(desc.size),data.fd,(uint64_t)pointer
	);
	handle=importbuffer_fd(data.fd,desc.size);
	if(!handle)throw RuntimeError("import dmabuf buffer failed");
	buffer=wrapbuffer_handle(handle,desc.width,desc.height,desc.format);
	dmabuf_fd=dmabuf_buf_f.Take();
}

void RGABuffer::Destroy(){
	if(handle)releasebuffer_handle(handle);
	if(allocated){
		if(pointer&&pointer!=MAP_FAILED)
			munmap(pointer,desc.size);
		if(dmabuf_fd>=0)close(dmabuf_fd);
	}
	pointer=nullptr,dmabuf_fd=-1;
}

void RGABuffers::CreateAll(int dmabuf_dev_fd){
	DestroyAll();
	try{
		for(size_t i=0;i<count;i++){
			auto c=std::make_shared<RGABuffer>();
			c->index=i,c->desc=desc;
			c->Create(dmabuf_dev_fd);
			buffers.push_back(c);
		}
	}catch(...){
		DestroyAll();
		throw;
	}
}

void RGABuffers::DestroyAll(){
	buffers.clear();
}
