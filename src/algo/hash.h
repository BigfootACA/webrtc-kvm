/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef HASH_H
#define HASH_H
#include<string>
#include<memory>
#include<cstdint>
#include<cstring>
class Hash{
	public:
		virtual Hash*Update(const void*data,size_t len)=0;
		[[nodiscard]] virtual uint8_t*ToBinary()=0;
		[[nodiscard]] virtual size_t GetHashBytes()=0;
		[[nodiscard]] virtual std::string GetName()=0;
		inline virtual Hash*Init(){return this;}
		inline virtual Hash*Reset(){return this;}
		inline Hash*operator+=(std::string&str){return Update(str);}
		[[nodiscard]] inline explicit operator std::string(){return ToString();}
		inline Hash*Update(const std::string&str){return Update(str.c_str(),str.length());}
		inline Hash*Update(const char*str){return Update(str,strlen(str));}
		virtual Hash*CopyTo(void*buffer){memcpy(buffer,ToBinary(),GetHashBytes());return this;}
		[[nodiscard]] inline virtual std::string ToString(){return ToLowerString();}
		[[nodiscard]] inline virtual std::string ToLowerString(){return ToHexStringByTable(lower_table);}
		[[nodiscard]] inline virtual std::string ToUpperString(){return ToHexStringByTable(upper_table);}
		[[nodiscard]] inline size_t GetHashBits(){return GetHashBytes()*8;}
		[[nodiscard]] inline size_t GetHashStringLen(){return GetHashBytes()*2;}
		[[nodiscard]] std::string ToHexStringByTable(const char*table);
		[[nodiscard]] static std::string BinaryToUpperHexString(const void*data,size_t len);
		[[nodiscard]] static std::string BinaryToLowerHexString(const void*data,size_t len);
		[[nodiscard]] static void*HexStringToBinary(const char*str,size_t len);
		[[nodiscard]] static void*HexStringToBinary(const char*str);
		[[nodiscard]] static void*HexStringToBinary(const std::string& str);
		static void HexStringToBinary(const char*str,void*data);
		static void HexStringToBinary(const std::string&str,void*data);
		static std::string BinaryToHexStringByTable(const char*table,const void*data,size_t len);
		static void HexStringToBinary(const char*str,size_t len,void*data);
		[[nodiscard]] static std::unique_ptr<Hash>NewHash(const std::string&type,const void*data=nullptr,size_t len=0);
		static void RandomBytes(void*buffer,size_t len);
		[[nodiscard]] static std::string RandomUpperHexString(size_t len);
		[[nodiscard]] static std::string RandomLowerHexString(size_t len);
		virtual ~Hash()=default;
	protected:
		static constexpr const char upper_table[]="0123456789ABCDEF";
		static constexpr const char lower_table[]="0123456789abcdef";
};
#endif
