/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef URL_H
#define URL_H
#include<string>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
class URL{
	public:
		URL(){Clean();}
		URL(const char*u,size_t len){Parse(u,len);}
		explicit URL(const char*u){Parse(u);}
		explicit URL(const std::string&str){Parse(str);}
		explicit URL(const std::string*str){Parse(str);}
		explicit URL(const URL&str){From(str);}
		explicit URL(const URL*str){From(str);}
		inline void Parse(const std::string*u){Parse(u->c_str(),u->length());}
		inline void Parse(const std::string&u){Parse(u.c_str(),u.length());}
		inline void Parse(const char*u){Parse(u,strlen(u));}
		void Parse(const char*u,size_t len);
		void Clean();
		bool GoBack();
		void From(const URL*url);
		inline void From(const URL&url){From(&url);}
		size_t AppendAll(std::string&val)const;
		size_t AppendOrigin(std::string&val)const;
		size_t AppendHierarchical(std::string&val)const;
		size_t AppendUserInfo(std::string&val)const;
		size_t AppendAuthority(std::string&val)const;
		size_t AppendFullPath(std::string&val)const;
		size_t AppendScheme(std::string&val)const;
		size_t AppendUsername(std::string&val)const;
		size_t AppendPassword(std::string&val)const;
		size_t AppendHost(std::string&val)const;
		size_t AppendPort(std::string&val)const;
		size_t AppendPath(std::string&val)const;
		size_t AppendQuery(std::string&val)const;
		size_t AppendFragment(std::string&val)const;
		void SetPort(const std::string&val);
		[[nodiscard]] size_t ToSockAddrIn(sockaddr*addr,int prefer=0)const;
		[[nodiscard]] inline size_t ToSockAddrIn(sockaddr_storage*addr,int prefer=0)const{
			return ToSockAddrIn((sockaddr*)addr,prefer);
		}
		[[nodiscard]] inline size_t ToSockAddrIn(sockaddr_in*addr,int prefer=AF_INET)const{
			return ToSockAddrIn((sockaddr*)addr,prefer);
		}
		[[nodiscard]] inline size_t ToSockAddrIn(sockaddr_in6*addr,int prefer=AF_INET6)const{
			return ToSockAddrIn((sockaddr*)addr,prefer);
		}
		[[nodiscard]] inline std::string GetOrigin()const{
			std::string out;AppendOrigin(out);return out;
		}
		[[nodiscard]] inline std::string GetHierarchical()const{
			std::string out;AppendHierarchical(out);return out;
		}
		[[nodiscard]] inline std::string GetUserInfo()const{
			std::string out;AppendUserInfo(out);return out;
		}
		[[nodiscard]] inline std::string GetAuthority()const{
			std::string out;AppendAuthority(out);return out;
		}
		[[nodiscard]] inline std::string GetFullPath()const{
			std::string out;AppendFullPath(out);return out;
		}
		[[nodiscard]] inline std::string GetURL()const{
			std::string out;AppendAll(out);return out;
		}
		[[nodiscard]] inline bool IsInTop()const{return path.empty()||path=="/";}
		[[nodiscard]] inline std::string ToString()const{return GetURL();}
		[[nodiscard]] inline std::string GetScheme()const{return scheme;}
		[[nodiscard]] inline std::string GetUsername()const{return username;}
		[[nodiscard]] inline std::string GetPassword()const{return password;}
		[[nodiscard]] inline std::string GetHost()const{return host;}
		[[nodiscard]] inline int GetPort()const{return port;}
		[[nodiscard]] inline std::string GetPath()const{return path;}
		[[nodiscard]] inline std::string GetQuery()const{return query;}
		[[nodiscard]] inline std::string GetFragment()const{return fragment;}
		inline void SetScheme(const std::string&val){scheme=val;}
		inline void SetUsernameDecoded(const std::string&val){username=val;}
		inline void SetPasswordDecoded(const std::string&val){password=val;}
		inline void SetHostDecoded(const std::string&val){host=val;}
		inline void SetPort(int val){port=val;}
		inline void SetPathDecoded(const std::string&val){path=val;}
		inline void SetQuery(const std::string&val){query=val;}
		inline void SetFragment(const std::string&val){fragment=val;}
		inline void SetUsername(const std::string&val){
			SetUsernameDecoded(Decode(val));
		}
		inline void SetPassword(const std::string&val){
			SetPasswordDecoded(Decode(val));
		}
		inline void SetHost(const std::string&val){
			SetHostDecoded(Decode(val));
		}
		inline void SetPath(const std::string&val){
			SetPathDecoded(Decode(val));
		}
		inline void SetScheme(const char*val){
			SetScheme(std::string(val));
		}
		inline void SetUsername(const char*val){
			SetUsername(std::string(val));
		}
		inline void SetPassword(const char*val){
			SetPassword(std::string(val));
		}
		inline void SetHost(const char*val){
			SetHost(std::string(val));
		}
		inline void SetPort(const char*val){
			SetPort(std::string(val));
		}
		inline void SetPath(const char*val){
			SetPath(std::string(val));
		}
		inline void SetQuery(const char*val){
			SetQuery(std::string(val));
		}
		inline void SetFragment(const char*val){
			SetFragment(std::string(val));
		}
		inline void SetUsernameDecoded(const char*val){
			SetUsernameDecoded(std::string(val));
		}
		inline void SetPasswordDecoded(const char*val){
			SetPasswordDecoded(std::string(val));
		}
		inline void SetHostDecoded(const char*val){
			SetHostDecoded(std::string(val));
		}
		inline void SetPathDecoded(const char*val){
			SetPathDecoded(std::string(val));
		}
		inline void SetScheme(const char*val,size_t len){
			SetScheme(std::string(val,len?:strlen(val)));
		}
		inline void SetUsername(const char*val,size_t len){
			SetUsername(std::string(val,len?:strlen(val)));
		}
		inline void SetPassword(const char*val,size_t len){
			SetPassword(std::string(val,len?:strlen(val)));
		}
		inline void SetHost(const char*val,size_t len){
			SetHost(std::string(val,len?:strlen(val)));
		}
		inline void SetPort(const char*val,size_t len){
			SetPort(std::string(val,len?:strlen(val)));
		}
		inline void SetPath(const char*val,size_t len){
			SetPath(std::string(val,len?:strlen(val)));
		}
		inline void SetQuery(const char*val,size_t len){
			SetQuery(std::string(val,len?:strlen(val)));
		}
		inline void SetFragment(const char*val,size_t len){
			SetFragment(std::string(val,len?:strlen(val)));
		}
		inline void SetUsernameDecoded(const char*val,size_t len){
			SetUsernameDecoded(std::string(val,len?:strlen(val)));
		}
		inline void SetPasswordDecoded(const char*val,size_t len){
			SetPasswordDecoded(std::string(val,len?:strlen(val)));
		}
		inline void SetHostDecoded(const char*val,size_t len){
			SetHostDecoded(std::string(val,len?:strlen(val)));
		}
		inline void SetPathDecoded(const char*val,size_t len){
			SetPathDecoded(std::string(val,len?:strlen(val)));
		}
		[[nodiscard]] int Compare(const URL*u)const;
		[[nodiscard]] int Compare(const std::string&u)const;
		[[nodiscard]] inline int Compare(const URL&u)const{return Compare(&u);}
		[[nodiscard]] inline int Compare(const char*str)const{return Compare(std::string(str));}
		[[nodiscard]] inline int Compare(const std::string*u)const{return Compare(*u);}
		[[nodiscard]] inline bool Equals(const char*str)const{return Compare(str)==0;}
		[[nodiscard]] inline bool Equals(const std::string*u)const{return Compare(u)==0;}
		[[nodiscard]] inline bool Equals(const std::string&u)const{return Compare(u)==0;}
		[[nodiscard]] inline bool Equals(const URL&u)const{return Compare(u)==0;}
		[[nodiscard]] inline bool Equals(const URL*u)const{return Compare(u)==0;}
		inline URL&operator=(const char*u){Parse(u);return *this;}
		inline URL&operator=(std::string*u){Parse(u);return *this;}
		inline URL&operator=(std::string&u){Parse(u);return *this;}
		inline URL&operator=(const URL*u){From(u);return *this;}
		inline URL&operator=(const URL&u){From(u);return *this;}
		[[nodiscard]] inline bool operator==(const char*u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator==(std::string*u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator==(std::string&u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator==(const URL*u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator==(const URL&u)const{return this->Equals(u);}
		[[nodiscard]] inline bool operator!=(const char*u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator!=(std::string*u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator!=(std::string&u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator!=(const URL*u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator!=(const URL&u)const{return !this->Equals(u);}
		[[nodiscard]] inline bool operator>(const URL*u)const{return this->Compare(u)>0;}
		[[nodiscard]] inline bool operator>(const URL&u)const{return this->Compare(u)>0;}
		[[nodiscard]] inline bool operator>=(const URL*u)const{return this->Compare(u)>=0;}
		[[nodiscard]] inline bool operator>=(const URL&u)const{return this->Compare(u)>=0;}
		[[nodiscard]] inline bool operator<(const URL*u)const{return this->Compare(u)<0;}
		[[nodiscard]] inline bool operator<(const URL&u)const{return this->Compare(u)<0;}
		[[nodiscard]] inline bool operator<=(const URL*u)const{return this->Compare(u)<=0;}
		[[nodiscard]] inline bool operator<=(const URL&u)const{return this->Compare(u)<=0;}
		inline std::ostream&operator<<(std::ostream&os)const{os<<ToString();return os;}
		[[nodiscard]] inline explicit operator std::string()const{return ToString();}
		[[nodiscard]] static std::string Encode(
			const std::string&src,
			const char*map=nullptr,
			const std::string&skip=""
		);
		[[nodiscard]] static std::string Decode(const std::string&src);
	private:
		std::string scheme;
		std::string username;
		std::string password;
		std::string host;
		int port=-1;
		std::string path;
		std::string query;
		std::string fragment;
};
#endif
