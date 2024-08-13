/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"buffer-queue.h"
#include<cstring>

void BufferQueue::AutoClean(){
	while(
		GetCount()>GetMaxCount()||
		GetLength()>GetMaxLength()
	)Pop();
}

void BufferQueue::Push(const std::shared_ptr<Blob>&payload){
	if(!payload)return;
	buffers.push(payload);
	length+=payload->GetLength();
	AutoClean();
}

void BufferQueue::Push(const void*ptr,size_t len){
	if(!ptr||len<=0)return;
	auto p=std::make_shared<Blob>(len);
	memcpy(p->ToPtr(),ptr,len);
	Push(p);
}

std::shared_ptr<Blob>BufferQueue::Pop(){
	if(buffers.empty())return nullptr;
	auto p=buffers.front();
	if(p)length-=p->GetLength();
	buffers.pop();
	return p;
}

std::shared_ptr<Blob> BufferQueue::First()const{
	if(buffers.empty())return nullptr;
	return buffers.front();
}

void BufferQueue::Clear(){
	while(!buffers.empty())Pop();
}
