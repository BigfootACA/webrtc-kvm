/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"buffer.h"
#include"lib/exception.h"
#include<cstring>
#include<utility>

Buffer::Buffer(const Buffer*buff){
	NewBuffer(buff->buf_size,buff->allocator);
	allow_auto_grow=buff->allow_auto_grow;
	allow_truncate=buff->allow_truncate;
	total=buff->total;
	PushData(buff);
}

Buffer::Buffer(std::shared_ptr<BufferAllocator>alloc){
	NewBuffer(BUFFER_DEFAULT_SIZE,std::move(alloc));
}

Buffer::Buffer(size_t size,std::shared_ptr<BufferAllocator>alloc){
	if(size>0)NewBuffer(size,std::move(alloc));
}

Buffer::Buffer(std::string&initial,std::shared_ptr<BufferAllocator>alloc){
	NewBuffer(BUFFER_DEFAULT_SIZE,std::move(alloc));
	PushData(initial);
}

Buffer::Buffer(const char*initial,std::shared_ptr<BufferAllocator>alloc){
	NewBuffer(BUFFER_DEFAULT_SIZE,std::move(alloc));
	PushData(initial);
}

Buffer::Buffer(char*buffer,size_t size,std::shared_ptr<BufferAllocator>alloc){
	SetBuffer(buffer,size,std::move(alloc));
}

char Buffer::At(size_t off)const{
	if(!buffer||buf_size<=0)return 0;
	if(off>=GetLength())throw InvalidArgument("bad offset");
	char*arr=GetContentPointer();
	return arr[off];
}

std::string Buffer::GetString(size_t off,size_t len)const{
	if(!buffer||buf_size<=0)return "";
	size_t length=GetLength();
	if(off>length)throw InvalidArgument("bad offset");
	length-=off;
	if(len>0)length=MIN(length,len);
	char*arr=GetContentPointer();
	return std::string(arr+off,length);
}

const char*Buffer::CopyTo(char*out,size_t size,size_t off,size_t len)const{
	if(!buffer||buf_size==0)return nullptr;
	if(!out||size==0||len==0)return out;
	size_t length=GetLength();
	if(off+len>=length)throw InvalidArgument("bad offset");
	if(len>0)length=MIN(length,len);
	char*arr=GetContentPointer();
	size_t copies=MIN(size-1,len);
	memcpy(out,arr+off,copies);
	out[copies]=0;
	return out;
}

void Buffer::DestroyBuffer(){
	Clear();
	if(allocator&&allocator->OnFree&&buffer&&buf_size>0)
		allocator->OnFree(buffer,buf_size);
	buffer=nullptr,buf_size=0,allocator=nullptr;
}

void Buffer::NewBuffer(size_t size,std::shared_ptr<BufferAllocator>alloc){
	if(!alloc)alloc=std::make_shared<BufferAllocator>(DefaultCAllocFree);
	if(!alloc||!alloc->OnNew)throw RuntimeError("allocate unsupported");
	void*buff=alloc->OnNew(size);
	if(!buff)throw RuntimeError("allocate failed");
	memset(buff,0,size);
	SetBuffer((char*)buff,size,alloc);
}

void Buffer::SetBuffer(char*buff,size_t size,std::shared_ptr<BufferAllocator>alloc){
	if(!alloc)alloc=std::make_shared<BufferAllocator>(DefaultCAllocFree);
	if(alloc&&!alloc->OnResize)allow_auto_grow=false;
	if(!buff||size<=0)throw InvalidArgument("bad buffer");
	DestroyBuffer();
	buffer=buff,buf_size=size,allocator=alloc;
	buffer[buf_size-1]=0;
}

size_t Buffer::GetLength()const{
	if(GetStartPos()>=GetEndPos())return 0;
	size_t current=GetSize()-GetStartPos();
	size_t length=GetEndPos()-GetStartPos();
	return MIN(length,current);
}

const char*Buffer::PopData(size_t len){
	if(len<=0)return GetContentPointer();
	len=round_up(len,buf_unit);
	if(len>=GetLength()){
		Clear();
		return GetContentPointer();
	}
	buf_start+=len;
	return GetContentPointer();
}

void Buffer::PushData(const char*data,size_t len){
	if(unlikely(!data))return;
	if(len==0)len=strlen(data);
	if(len>GetAvailable())Trim();
	if(len>GetAvailable()&&allow_auto_grow)
		Grow(round_up(len,BUFFER_MIN_ADDEND));
	if(len>GetAvailable()&&allow_truncate){
		for(const auto&callback:on_discard)callback();
		WantAvailable(len);
	}
	if(len>GetSize()&&allow_truncate){
		size_t size=GetSize();
		data+=len-size,len=size;
		Clear();
	}
	if(unlikely(len>GetAvailable()))
		throw RuntimeError("no available space in buffer");
	if(unlikely(buf_end+len>=buf_size))
		throw InvalidArgument("unexpected buffer overflow");
	char*ptr=GetEndPointer();
	memcpy(ptr,data,len);
	buf_end+=len,total+=len;
}

ssize_t Buffer::IndexOf(const char*str,size_t len)const{
	if(unlikely(!str))return 0;
	if(len==0)len=strlen(str);
	char*buff=GetContentPointer();
	size_t cnt=GetLength();
	if(unlikely(!buff))return -1;
	void*found=memmem(buff,cnt,str,len);
	if(unlikely(!found))return -1;
	return (ssize_t)found-(ssize_t)buff;
}

int Buffer::Compare(const Buffer*that)const{
	size_t l1=this->GetLength();
	size_t l2=that->GetLength();
	char*s1=this->GetContentPointer();
	char*s2=that->GetContentPointer();
	if(that==this)return 0;
	if(s1==s2&&l1==l2)return 0;
	if(!s1||!s2)return -1;
	int r=memcmp(s1,s2,MIN(l1,l2));
	if(r<0)return -1;
	if(r>0)return 1;
	if(l1<l2)return -1;
	if(l1>l2)return 1;
	return 0;
}
void Buffer::TruncateTo(const char*str,size_t len){
	ssize_t r=IndexOf(str,len);
	if(r<=0)return;
	PopData(r);
}

void Buffer::Trim(){
	if(buf_start==0||!buffer||buf_size<=0)return;
	size_t length=GetLength();
	if(length>0)memcpy(GetBuffer(),GetContentPointer(),length);
	buf_start=0,buf_end=length;
}

void Buffer::Resize(size_t size){
	if(!buffer||buf_size==0)NewBuffer(size);
	if(buf_max>0)size=MIN(size,buf_max);
	if(size==buf_size)return;
	if(!allocator||!allocator->OnResize)
		throw InvalidArgument("resize unsupported");
	Trim();
	if(size<GetAvailable())
		throw RuntimeError("buffer too small to store current data");
	void*buff=allocator->OnResize(buffer,buf_size,size);
	if(!buff)throw RuntimeError("resize buffer failed");
	buffer=(char*)buff,buf_size=size;
	buffer[buf_size-1]=0;
	Trim();
}

void Buffer::WantAvailable(size_t available){
	Trim();
	size_t avail=GetAvailable();
	if(avail>=available)return;
	PopData(available-avail);
	Trim();
}

size_t Buffer::GetMaxAvailable()const{
	if(allow_auto_grow)return SIZE_MAX;
	return GetSize()-GetLength()-1;
}

void Buffer::Truncate(size_t length){
	size_t len=GetLength();
	if(len>length)PopData(len-length);
}

void Buffer::TruncateTo(const std::string&str){
	TruncateTo(str.c_str(),str.length());
}

void Buffer::Grow(size_t addend){
	addend=round_up(addend,buf_unit);
	if(addend>SIZE_MAX-buf_size)
		throw InvalidArgument("grow too large");
	Resize(buf_size+addend);
}

void Buffer::Shrink(size_t subtrahend){
	subtrahend=round_down(subtrahend,buf_unit);
	if(subtrahend>=buf_size)
		throw InvalidArgument("shrink too small");
	Resize(buf_size-subtrahend);
}

void Buffer::SetAllowAutoGrow(bool allow){
	if(allow&&(!allocator||!allocator->OnResize))
		throw InvalidArgument("resize unsupported");
	allow_auto_grow=allow;
}

BufferAllocator Buffer::DefaultCAllocFree={
	.OnNew=[](size_t size)->void*{return malloc(size);},
	.OnFree=[](void*buff,size_t size)->void{free(buff);},
	.OnResize=[](void*buff,size_t old_size,size_t new_size)->void*{
		if(old_size==new_size)return buff;
		if(!buff)return malloc(new_size);
		return realloc(buff,new_size);
	},
};

