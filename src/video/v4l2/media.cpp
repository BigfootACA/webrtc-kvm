/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include<linux/media.h>

int media_get_topology(int fd,media_v2_topology**topology){
	int ret=-ENOMEM;
	media_v2_topology*t=nullptr;
	if(!topology)return set_errno(EINVAL);
	*topology=nullptr;
	if(!(t=(struct media_v2_topology*)malloc(sizeof(struct media_v2_topology))))
		goto_printf("alloc topology failed\n");
	memset(t,0,sizeof(struct media_v2_topology));
	if((ret=ioctl(fd,MEDIA_IOC_G_TOPOLOGY,t)))
		goto_printf("get topology failed: %m\n");
		#define ALLOC_ENTRIES(val,type)\
		if(!t->ptr_##val){\
			size_t size=sizeof(struct type)*t->num_##val;\
			void*ptr=malloc(size);\
			if(!ptr)goto_printf("alloc "#val" failed\n");\
			memset(ptr,0,size);\
			t->ptr_##val=(uintptr_t)ptr;\
		}
	ALLOC_ENTRIES(pads,media_v2_pad)
	ALLOC_ENTRIES(links,media_v2_link)
	ALLOC_ENTRIES(entities,media_v2_entity)
	ALLOC_ENTRIES(interfaces,media_v2_interface)
	if((ret=ioctl(fd,MEDIA_IOC_G_TOPOLOGY,t)))
		goto_printf("get topology failed: %m\n");
	*topology=t;
	return 0;
	error:
	if(t){
		if(t->ptr_pads)free((void*)(uintptr_t)t->ptr_pads);
		if(t->ptr_links)free((void*)(uintptr_t)t->ptr_links);
		if(t->ptr_entities)free((void*)(uintptr_t)t->ptr_entities);
		if(t->ptr_interfaces)free((void*)(uintptr_t)t->ptr_interfaces);
		free(t);
	}
	return ret;
}
