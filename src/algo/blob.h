/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef BLOB_H
#define BLOB_H
#include<string>
#include<cstddef>
#include<cstdint>
class Blob{
	public:
		~Blob();
		explicit Blob(size_t len);
		explicit Blob(const std::string&str,bool copy=false);
		Blob(void*ptr,size_t len,bool alloc=false);
		void Resize(size_t len);
		inline void AddSize(size_t len){Resize(this->len+len);}
		template<typename T> [[nodiscard]] inline T As()const{return reinterpret_cast<T>(buffer);}
		inline void SetLength(size_t len){this->len=len;}
		[[nodiscard]] inline size_t GetLength()const{return len;}
		[[nodiscard]] inline void*ToPtr()const{return (void*)buffer;}
		[[nodiscard]] inline std::byte*ToBytes()const{return (std::byte*)buffer;}
		[[nodiscard]] inline char*ToCharPtr()const{return (char*)buffer;}
		[[nodiscard]] inline int8_t*ToInt8Ptr()const{return (int8_t*)buffer;}
		[[nodiscard]] inline int16_t*ToInt16Ptr()const{return (int16_t*)buffer;}
		[[nodiscard]] inline int32_t*ToInt32Ptr()const{return (int32_t*)buffer;}
		[[nodiscard]] inline int64_t*ToInt64Ptr()const{return (int64_t*)buffer;}
		[[nodiscard]] inline uint8_t*ToUInt8Ptr()const{return (uint8_t*)buffer;}
		[[nodiscard]] inline uint16_t*ToUInt16Ptr()const{return (uint16_t*)buffer;}
		[[nodiscard]] inline uint32_t*ToUInt32Ptr()const{return (uint32_t*)buffer;}
		[[nodiscard]] inline uint64_t*ToUInt64Ptr()const{return (uint64_t*)buffer;}
		[[nodiscard]] inline std::string ToString()const{return std::string((char*)buffer,len);}
		[[nodiscard]] inline unsigned char*ToUCharPtr()const{return (unsigned char*)buffer;}
		[[nodiscard]] inline bool IsEmpty()const{return !buffer||len==0;}
		[[nodiscard]] inline explicit operator std::string()const{return ToString();}
		[[nodiscard]] inline uint8_t*begin(){return &buffer[0];}
		[[nodiscard]] inline const uint8_t*begin()const{return &buffer[0];}
		[[nodiscard]] inline uint8_t*end(){return &buffer[len];}
		[[nodiscard]] inline const uint8_t*end()const{return &buffer[len];}
		[[nodiscard]] inline explicit operator std::string(){return std::string((char*)buffer,len);}
		uint8_t&operator[](size_t idx);
		const uint8_t&operator[](size_t idx)const;
	private:
		size_t len=0;
		bool alloc=false;
		uint8_t*buffer=nullptr;
};
#endif
