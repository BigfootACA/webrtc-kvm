/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef BUFFER_QUEUE_H
#define BUFFER_QUEUE_H
#include<mutex>
#include<queue>
#include<memory>
#include<cstddef>
#include<cstdint>
#include<utility>
#include"algo/blob.h"
class BufferQueue{
	public:
		inline BufferQueue()=default;
		[[nodiscard]] inline size_t GetMaxLength()const{return max_length;}
		[[nodiscard]] inline size_t GetMaxCount()const{return max_count;}
		inline void SetMaxLength(size_t len){max_length=len;AutoClean();}
		inline void SetMaxCount(size_t count){max_count=count;AutoClean();}
		void AutoClean();
		[[nodiscard]] size_t GetLength()const{return length;}
		[[nodiscard]] size_t GetCount()const{return buffers.size();}
		void Push(const std::shared_ptr<Blob>&payload);
		void Push(const void*ptr,size_t len);
		void Clear();
		std::shared_ptr<Blob>Pop();
		[[nodiscard]] std::shared_ptr<Blob>First()const;
		[[nodiscard]] bool Empty()const{return buffers.empty();}
		inline void operator+=(const std::shared_ptr<Blob>&payload){Push(payload);}
		inline std::shared_ptr<Blob>operator--(){return Pop();}
		std::mutex lock;
	private:
		size_t max_length=SIZE_MAX;
		size_t max_count=SIZE_MAX;
		size_t length=0;
		std::queue<std::shared_ptr<Blob>>buffers;
};
#endif
