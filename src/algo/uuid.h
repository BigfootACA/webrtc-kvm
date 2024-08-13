/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef UUID_H
#define UUID_H
#include<string>
#include<cstdio>
#include<cstring>
#include<cstdint>
#include<iostream>
#include<uuid/uuid.h>

class UUID{
	public:
		inline UUID(){Clear();}
		inline UUID(const UUID&u){Set(u);}
		inline explicit UUID(const UUID*u){Set(u);}
		inline explicit UUID(const uuid_t*u){Set(u);}
		inline explicit UUID(const uuid_t&u){Set(u);}
		inline explicit UUID(const std::string*u){Set(u);}
		inline explicit UUID(const std::string&u){Set(u);}
		inline explicit UUID(const char*u){Set(u);}
		inline explicit UUID(const void*u){Set(u);}
		inline UUID(
			uint8_t b0,uint8_t b1,uint8_t b2,uint8_t b3,
			uint8_t b4,uint8_t b5,uint8_t b6,uint8_t b7,
			uint8_t b8,uint8_t b9,uint8_t b10,uint8_t b11,
			uint8_t b12,uint8_t b13,uint8_t b14,uint8_t b15
		){Set(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15);}
		void Generate();
		void GenerateTime();
		void GenerateTimeSafe();
		void GenerateRandom();
		void GenerateMD5(const UUID&ns,const std::string&name);
		void GenerateSHA1(const UUID&ns,const std::string&name);
		inline void Set(const UUID*u){Set((const void*)u->uuid);}
		inline void Set(const UUID&u){Set((const void*)u.uuid);}
		inline void Set(const uuid_t*u){Set((const void*)*u);}
		inline void Set(const uuid_t&u){Set((const void*)u);}
		inline void Set(const std::string*u){Set(u->c_str(),u->length());}
		inline void Set(const std::string&u){Set(u.c_str(),u.length());}
		inline void Set(const char*u){Set(u,strlen(u));}
		void Set(const char*u,size_t len);
		void Set(const void*u);
		void Set(
			uint8_t b0,uint8_t b1,uint8_t b2,uint8_t b3,
			uint8_t b4,uint8_t b5,uint8_t b6,uint8_t b7,
			uint8_t b8,uint8_t b9,uint8_t b10,uint8_t b11,
			uint8_t b12,uint8_t b13,uint8_t b14,uint8_t b15
		){Set((uuid_t){b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15});}
		void Clear();
		[[nodiscard]] bool IsNull()const;
		[[nodiscard]] int Compare(const void*u)const;
		[[nodiscard]] inline int Compare(const char*str)const{return Compare(UUID(str));}
		[[nodiscard]] inline int Compare(const UUID*u)const{return Compare(u->uuid);}
		[[nodiscard]] inline int Compare(const UUID&u)const{return Compare(u.uuid);}
		[[nodiscard]] inline int Compare(const uuid_t*u)const{return Compare((const void*)*u);}
		[[nodiscard]] inline int Compare(const uuid_t&u)const{return Compare((const void*)u);}
		[[nodiscard]] inline int Compare(const std::string*u)const{return Compare(UUID(u));}
		[[nodiscard]] inline int Compare(const std::string&u)const{return Compare(UUID(u));}
		[[nodiscard]] inline bool Equals(const char*str)const{return Compare(str)==0;}
		[[nodiscard]] inline bool Equals(const uuid_t*u)const{return Compare(u)==0;}
		[[nodiscard]] inline bool Equals(const uuid_t&u)const{return Compare(u)==0;}
		[[nodiscard]] inline bool Equals(const UUID*u)const{return Compare(u)==0;}
		[[nodiscard]] inline bool Equals(const UUID&u)const{return Compare(u)==0;}
		[[nodiscard]] inline bool Equals(const std::string*u)const{return Compare(u)==0;}
		[[nodiscard]] inline bool Equals(const std::string&u)const{return Compare(u)==0;}
		[[nodiscard]] inline void*ToBinary()const{return (void*)this->uuid;}
		[[nodiscard]] std::string ToString()const;
		[[nodiscard]] std::string ToUpperString()const;
		[[nodiscard]] std::string ToLowerString()const;
		[[nodiscard]] int GetType()const;
		[[nodiscard]] int GetVariant()const;
		inline UUID&operator=(const char*u){Set(u);return *this;}
		inline UUID&operator=(const std::string*u){Set(u);return *this;}
		inline UUID&operator=(const std::string&u){Set(u);return *this;}
		inline UUID&operator=(const UUID*u){Set(u);return *this;}
		inline UUID&operator=(const UUID&u){Set(u);return *this;}
		inline UUID&operator=(const uuid_t*u){Set(u);return *this;}
		inline UUID&operator=(const uuid_t&u){Set(u);return *this;}
		[[nodiscard]] inline bool operator==(const char*u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator==(const std::string*u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator==(const std::string&u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator==(const UUID*u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator==(const UUID&u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator==(const uuid_t*u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator==(const uuid_t&u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator!=(const char*u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator!=(const std::string*u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator!=(const std::string&u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator!=(const UUID*u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator!=(const UUID&u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator!=(const uuid_t*u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator!=(const uuid_t&u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator>(const UUID*u)const{return this->Compare(u)>0;}
		[[nodiscard]] inline bool operator>(const UUID&u)const{return this->Compare(u)>0;}
		[[nodiscard]] inline bool operator>=(const UUID*u)const{return this->Compare(u)>=0;}
		[[nodiscard]] inline bool operator>=(const UUID&u)const{return this->Compare(u)>=0;}
		[[nodiscard]] inline bool operator<(const UUID*u)const{return this->Compare(u)<0;}
		[[nodiscard]] inline bool operator<(const UUID&u)const{return this->Compare(u)<0;}
		[[nodiscard]] inline bool operator<=(const UUID*u)const{return this->Compare(u)<=0;}
		[[nodiscard]] inline bool operator<=(const UUID&u)const{return this->Compare(u)<=0;}
		inline std::ostream&operator<<(std::ostream&os)const{os<<ToString();return os;}
		[[nodiscard]] inline explicit operator std::string()const{return ToString();}
		[[nodiscard]] static UUID GetTemplate(const std::string&name);
		[[nodiscard]] static UUID Random(){UUID u;u.Generate();return u;}
	private:
		uuid_t uuid{};
};
#endif
