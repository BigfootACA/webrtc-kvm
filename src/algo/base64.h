/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef BASE64_H
#define BASE64_H
#include<string>
#include<cstring>
#include"blob.h"
class Base64{
	public:
		[[nodiscard]] static std::string encode(const void*data,size_t len);
		[[nodiscard]] static std::string decode(const void*data,size_t len);
		[[nodiscard]] static inline std::string encode(const std::string&str){return encode(str.c_str(),str.length());}
		[[nodiscard]] static inline std::string decode(const std::string&str){return decode(str.c_str(),str.length());}
		[[nodiscard]] static inline std::string encode(const Blob&blob){return encode(blob.ToPtr(),blob.GetLength());}
		[[nodiscard]] static inline std::string decode(const Blob&blob){return decode(blob.ToPtr(),blob.GetLength());}
		[[nodiscard]] static inline std::string encode(const char*str){return encode(str,strlen(str));}
		[[nodiscard]] static inline std::string decode(const char*str){return decode(str,strlen(str));}
};
#endif
