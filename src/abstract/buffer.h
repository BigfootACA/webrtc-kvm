/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef BUFFER_H
#define BUFFER_H
#include<memory>
#include<string>
#include<functional>
#include<shared_mutex>
#include"defines.h"
#include"algo/blob.h"
#define BUFFER_DEFAULT_SIZE 0x2000
#define BUFFER_MIN_ADDEND 0x1000
struct BufferAllocator{
	std::function<void*(size_t size)>OnNew;
	std::function<void(void*buffer,size_t size)>OnFree;
	std::function<void*(void*buffer,size_t old_size,size_t new_size)>OnResize;
};
class Buffer{
	public:
		explicit Buffer(const Buffer*buff);
		inline Buffer(const Buffer&buff):Buffer(&buff){}
		explicit Buffer(std::shared_ptr<BufferAllocator>alloc=nullptr);
		explicit Buffer(size_t size,std::shared_ptr<BufferAllocator>alloc=nullptr);
		explicit Buffer(std::string&initial,std::shared_ptr<BufferAllocator>alloc=nullptr);
		explicit Buffer(const char*initial,std::shared_ptr<BufferAllocator>alloc=nullptr);
		Buffer(char*buffer,size_t size,std::shared_ptr<BufferAllocator>alloc=nullptr);
		inline ~Buffer(){DestroyBuffer();}
		void DestroyBuffer();
		void NewBuffer(size_t size=BUFFER_DEFAULT_SIZE,std::shared_ptr<BufferAllocator>alloc=nullptr);
		void SetBuffer(char*buff,size_t size,std::shared_ptr<BufferAllocator>alloc=nullptr);
		[[nodiscard]] size_t GetLength()const;
		void Trim();
		void Resize(size_t size);
		[[nodiscard]] char At(size_t off=0)const;
		[[nodiscard]] std::string GetString(size_t off=0,size_t len=0)const;
		const char*CopyTo(char*out,size_t size,size_t off=0,size_t len=0)const;
		const char*PopData(size_t len);
		void PushData(const char*data,size_t len=0);
		inline void PushData(const std::string&data){PushData(data.c_str(),data.length());}
		inline void PushData(const Buffer*data){PushData(data->GetContentPointer(),data->GetLength());}
		inline void PushData(const Buffer&data){PushData(&data);}
		inline void Clear(){buf_start=0,buf_end=0;}
		[[nodiscard]] inline Blob GetBlob()const{return Blob(buffer,buf_size);}
		[[nodiscard]] inline char*GetBuffer()const{return buffer;}
		[[nodiscard]] inline size_t GetAvailable()const{return GetSize()-GetEndPos()-1;}
		[[nodiscard]] inline char*GetContentPointer()const{return GetStartPointer();}
		[[nodiscard]] inline char*GetStartPointer()const{return GetBuffer()+GetStartPos();}
		[[nodiscard]] inline char*GetEndPointer()const{return GetBuffer()+GetEndPos();}
		[[nodiscard]] inline size_t GetStartPos()const{return buffer?MIN(buf_start,buf_size):0;}
		[[nodiscard]] inline size_t GetEndPos()const{return buffer?MIN(buf_end,buf_size):0;}
		[[nodiscard]] inline size_t GetSize()const{return buffer?buf_size:0;}
		void WantAvailable(size_t available);
		void TruncateTo(const char*str,size_t len=0);
		[[nodiscard]] size_t GetMaxAvailable()const;
		void Truncate(size_t length);
		void TruncateTo(const std::string&str);
		void Grow(size_t addend=BUFFER_MIN_ADDEND);
		void Shrink(size_t subtrahend);
		void SetAllowAutoGrow(bool allow);
		int Compare(const Buffer*that)const;
		[[nodiscard]] inline int Compare(const Buffer&that)const{return Compare(&that);}
		[[nodiscard]] ssize_t IndexOf(const char*str,size_t len=0)const;
		[[nodiscard]] inline ssize_t IndexOf(const std::string&str)const{return IndexOf(str.c_str(),str.length());}
		[[nodiscard]] inline bool Contains(const char*str,size_t len=0)const{return IndexOf(str,len)>=0;}
		[[nodiscard]] inline bool Contains(const std::string&str)const{return Contains(str.c_str(),str.length());}
		[[nodiscard]] inline bool IsAllowAutoGrow()const{return allow_auto_grow;}
		[[nodiscard]] inline bool IsAllowTruncate()const{return allow_truncate;}
		[[nodiscard]] inline size_t GetMaxSize()const{return buf_max;}
		[[nodiscard]] inline size_t GetUnitSize()const{return buf_unit;}
		[[nodiscard]] inline size_t GetTotal()const{return total;}
		inline void SetAllowTruncate(bool allow){allow_truncate=allow;}
		inline void SetMaxSize(size_t limit){buf_max=limit;}
		inline void SetUnitSize(size_t unit){buf_unit=unit;}
		inline void AddOnDiscard(const std::function<void()>&cb){on_discard.push_back(cb);}
		inline bool operator==(const Buffer*that)const{return this->Compare(that)==0;}
		inline bool operator!=(const Buffer*that)const{return this->Compare(that)!=0;}
		inline bool operator<(const Buffer*that)const{return this->Compare(that)<0;}
		inline bool operator<=(const Buffer*that)const{return this->Compare(that)<=0;}
		inline bool operator>(const Buffer*that)const{return this->Compare(that)>0;}
		inline bool operator>=(const Buffer*that)const{return this->Compare(that)>=0;}
		inline bool operator==(const Buffer&that)const{return this->Compare(that)==0;}
		inline bool operator!=(const Buffer&that)const{return this->Compare(that)!=0;}
		inline bool operator<(const Buffer&that)const{return this->Compare(that)<0;}
		inline bool operator<=(const Buffer&that)const{return this->Compare(that)<=0;}
		inline bool operator>(const Buffer&that)const{return this->Compare(that)>0;}
		inline bool operator>=(const Buffer&that)const{return this->Compare(that)>=0;}
		inline void operator+=(const Buffer&data){PushData(data);}
		inline void operator+=(const Buffer*data){PushData(data);}
		inline void operator+=(const char*data){PushData(data);}
		inline void operator-=(size_t len){PopData(len);}
		inline char operator[](size_t off){return At(off);}
		inline std::ostream&operator<<(std::ostream&os)const{os<<GetString();return os;}
		inline explicit operator void*()const{return GetContentPointer();}
		inline explicit operator const char*()const{return GetContentPointer();}
		inline explicit operator std::string()const{return GetString();}
		static BufferAllocator DefaultCAllocFree;
		std::shared_mutex lock;
	protected:
		bool allow_auto_grow=false;
		bool allow_truncate=false;
		std::shared_ptr<BufferAllocator>allocator;
		std::vector<std::function<void()>>on_discard;
		char*buffer=nullptr;
		size_t buf_unit=1;
		size_t buf_size=0;
		size_t buf_start=0;
		size_t buf_end=0;
		size_t buf_max=0;
		size_t total=0;
};
#endif
