/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef HTTP_H
#define HTTP_H
#include<map>
#include<string>
#include<microhttpd.h>
#include<json/json.h>
#include"webrtc_kvm.h"
#include"lib/exception.h"
#include"algo/uuid.h"
#include"algo/blob.h"
#include"main/builtin.h"
#include"lib/log.h"

#define HTTP_POST_STEP 4096
#define HTTP_POST_MAX 16384
#define HTTP_AUTH_VALID_TIME 7*24*60*60
#define MHD_SKIP ((MHD_Result)INT_MAX)

class HttpAuthStatus;

struct post_data{
	size_t size,len;
	bool overflow;
	char payload[];
};

enum HttpVersion{
	HTTP_VERSION_UNKNOWN = 0,
	HTTP_VERSION_0_9     = BIT(0),
	HTTP_VERSION_1_0     = BIT(1),
	HTTP_VERSION_1_1     = BIT(2),
	HTTP_VERSION_2_0     = BIT(3),
	HTTP_VERSION_3_0     = BIT(4),
};

enum HttpMethod{
	HTTP_METHOD_UNKNOWN           = 0,
	HTTP_METHOD_GET               = BIT(0),
	HTTP_METHOD_HEAD              = BIT(1),
	HTTP_METHOD_POST              = BIT(2),
	HTTP_METHOD_PUT               = BIT(3),
	HTTP_METHOD_DELETE            = BIT(4),
	HTTP_METHOD_CONNECT           = BIT(5),
	HTTP_METHOD_OPTIONS           = BIT(6),
	HTTP_METHOD_TRACE             = BIT(7),
	HTTP_METHOD_ACL               = BIT(8),
	HTTP_METHOD_BASELINE_CONTROL  = BIT(9),
	HTTP_METHOD_BIND              = BIT(10),
	HTTP_METHOD_CHECKIN           = BIT(11),
	HTTP_METHOD_CHECKOUT          = BIT(12),
	HTTP_METHOD_COPY              = BIT(13),
	HTTP_METHOD_LABEL             = BIT(14),
	HTTP_METHOD_LINK              = BIT(15),
	HTTP_METHOD_LOCK              = BIT(16),
	HTTP_METHOD_MERGE             = BIT(17),
	HTTP_METHOD_MKACTIVITY        = BIT(18),
	HTTP_METHOD_MKCALENDAR        = BIT(19),
	HTTP_METHOD_MKCOL             = BIT(20),
	HTTP_METHOD_MKREDIRECTREF     = BIT(21),
	HTTP_METHOD_MKWORKSPACE       = BIT(22),
	HTTP_METHOD_MOVE              = BIT(23),
	HTTP_METHOD_ORDERPATCH        = BIT(24),
	HTTP_METHOD_PATCH             = BIT(25),
	HTTP_METHOD_PRI               = BIT(26),
	HTTP_METHOD_PROPFIND          = BIT(27),
	HTTP_METHOD_PROPPATCH         = BIT(28),
	HTTP_METHOD_REBIND            = BIT(29),
	HTTP_METHOD_REPORT            = BIT(30),
	HTTP_METHOD_SEARCH            = BIT(31),
	HTTP_METHOD_UNBIND            = BIT(32),
	HTTP_METHOD_UNCHECKOUT        = BIT(33),
	HTTP_METHOD_UNLINK            = BIT(34),
	HTTP_METHOD_UNLOCK            = BIT(35),
	HTTP_METHOD_UPDATE            = BIT(36),
	HTTP_METHOD_UPDATEREDIRECTREF = BIT(37),
	HTTP_METHOD_VERSION_CONTROL   = BIT(38),
	HTTP_METHOD_ASTERISK          = BIT(39),
};

class HttpHeaders{
	public:
		inline void Clear(){headers.clear();}
		inline bool Empty()const{return headers.empty();}
		void Add(const std::string&name,const std::string&value);
		void Ensure(const std::string&name,const std::string&value);
		void Replace(const std::string&name,const std::string&value);
		void Remove(const std::string&name);
		bool Have(const std::string&name)const;
		void FillToResponse(MHD_Response*res)const;
		void FillFromConnection(MHD_Connection*conn);
		std::string GetOne(const std::string&name)const;
		std::vector<std::string>Get(const std::string&name)const;
		std::vector<std::string>GetList(const std::string&name)const;
		inline std::vector<std::pair<std::string,std::string>>All(){return headers;}
		inline std::vector<std::pair<std::string,std::string>>::iterator begin(){return headers.begin();}
		inline std::vector<std::pair<std::string,std::string>>::const_iterator begin()const{return headers.begin();}
		inline std::vector<std::pair<std::string,std::string>>::iterator end(){return headers.end();}
		inline std::vector<std::pair<std::string,std::string>>::const_iterator end()const{return headers.end();}
		inline std::vector<std::pair<std::string,std::string>>::reference operator[](size_t n)noexcept{return headers[n];}
		inline std::vector<std::pair<std::string,std::string>>::const_reference operator[](size_t n)const noexcept{return headers[n];}
		static std::string TimeToString(time_t t=time(nullptr));
		static time_t StringToTime(const std::string&gmt);
		static std::vector<std::string>ParseList(const std::string&var);
		static std::vector<std::string>ParseList(const std::vector<std::string>list);
	private:
		std::vector<std::pair<std::string,std::string>>headers;
};

class HttpRequestContext{
	public:
		HttpRequestContext(webrtc_kvm*ctx,MHD_Connection*conn);
		webrtc_kvm*ctx=nullptr;
		MHD_Connection*conn=nullptr;
		std::string url;
		HttpMethod method;
		HttpVersion version;
		const char*data;
		size_t*data_size=nullptr;
		void**con_cls=nullptr;
		HttpAuthorization*auth=nullptr;
		std::shared_ptr<HttpAuthStatus>token=nullptr;
		HttpHeaders res_headers;
		HttpHeaders req_headers;
		MHD_Result HandleJSON(int code,Json::Value&val);
		MHD_Result HandleData(int code,const std::string&mime,void*body,size_t len,MHD_ResponseMemoryMode mode);
		MHD_Result HandleString(int code,const std::string&mime,const std::string&val);
		MHD_Result CatchRun(std::function<MHD_Result(HttpRequestContext*ctx)>run);
};

class HttpPOSTRequestContext:public HttpRequestContext{
	public:
		HttpPOSTRequestContext(HttpRequestContext*p,post_data*pd);
		[[nodiscard]] inline std::string content()const{return post?std::string(post->payload,post->size):"";}
		post_data*post=nullptr;
};

class HttpRestfulRequestContext:public HttpPOSTRequestContext{
	public:
		explicit HttpRestfulRequestContext(HttpPOSTRequestContext*p);
		Json::Value request;
		Json::Value response;
};

class HttpHandler{
	public:
		[[nodiscard]] virtual std::string GetPath()const=0;
		[[nodiscard]] virtual HttpMethod GetMethod()const{return HTTP_METHOD_UNKNOWN;}
		[[nodiscard]] virtual bool NeedAuth()const{return false;}
		MHD_Result Process(HttpRequestContext*ctx);
		virtual MHD_Result Handle(HttpRequestContext*ctx)=0;
		void RegisterSelf();
		static HttpHandler*GetHandler(const std::string&url);
		static MHD_Result HandleConnection(
			void*cls,
			MHD_Connection*conn,
			const char*url,
			const char*method,
			const char*version,
			const char*data,
			size_t*data_size,
			void**con_cls
		);
};

class HttpPOSTHandler:public HttpHandler{
	public:
		MHD_Result Handle(HttpRequestContext*ctx)final;
		MHD_Result FetchPost(HttpRequestContext*ctx);
		virtual MHD_Result HandlePOST(HttpPOSTRequestContext*ctx)=0;
};

class HttpRestfulHandler:public HttpPOSTHandler{
	public:
		MHD_Result HandlePOST(HttpPOSTRequestContext*ctx)final;
		virtual int HandleRestful(HttpRestfulRequestContext*ctx)=0;
};

class HttpCompress{
	public:
		HttpCompress(const std::string&name,const std::shared_ptr<Blob>blob,MHD_ResponseMemoryMode mode);
		MHD_Result TryCompress(const std::string&mime,HttpRequestContext*ctx);
	private:
		std::shared_ptr<Blob>GenerateCompress(const std::string&type);
		std::string name;
		std::shared_ptr<Blob>file;
		std::vector<std::string>skipped;
		std::map<std::string,std::shared_ptr<Blob>>cache;
		MHD_ResponseMemoryMode mode;
};

class HttpStaticHandler:public HttpHandler{
	public:
		HttpStaticHandler(const std::string&path,const std::string&mime,const void*data,size_t len);
		HttpStaticHandler(const std::string&path,const std::string&mime,const std::shared_ptr<Blob>blob);
		HttpStaticHandler(const std::string&path,const void*data,size_t len);
		HttpStaticHandler(const std::string&path,const std::shared_ptr<Blob>blob);
		[[nodiscard]] std::string GetPath()const final{return path;}
		MHD_Result Handle(HttpRequestContext*ctx)final;
		MHD_Result HandleUnmodified(HttpRequestContext*ctx);
		MHD_Result TryModified(HttpRequestContext*ctx);
	private:
		std::string path,mime;
		std::shared_ptr<Blob>file;
		std::shared_ptr<HttpCompress>comp;
		time_t modified;
};

class HttpStaticListHandler{
	public:
		HttpStaticListHandler(const std::string&path,const file_item*items);
	private:
		std::vector<std::shared_ptr<HttpStaticHandler>>handlers;
};

class HttpAuthStatus{
	public:
		HttpAuthStatus(const UUID&token,const std::string&user,time_t time);
		inline UUID GetToken()const{return token;}
		inline std::string GetUser()const{return user;}
		inline time_t GetTime()const{return login_time;}
		bool Valid()const;
	private:
		UUID token;
		std::string user;
		time_t login_time;
};

struct AuthRequest{
	std::string username;
	std::string password;
};

class HttpAuthorization{
	public:
		HttpAuthorization(webrtc_kvm*ctx);
		void Logout(const UUID&token);
		void Logout(std::shared_ptr<HttpAuthStatus>token);
		std::shared_ptr<HttpAuthStatus>Login(const AuthRequest&req);
		std::shared_ptr<HttpAuthStatus>GetAuthToken(const UUID&token);
		std::shared_ptr<HttpAuthStatus>GetAuthTokenFromHeader(const HttpHeaders&hdrs);
	private:
		webrtc_kvm*ctx;
		void AuthPAM(const AuthRequest&req);
		void AuthPassword(const AuthRequest&req);
		std::map<UUID,std::shared_ptr<HttpAuthStatus>>tokens;
};

extern bool StringToHttpMethod(const std::string&val,HttpMethod&method);
extern bool StringToHttpVersion(const std::string&val,HttpVersion&version);
extern std::string HttpMethodToString(HttpMethod method);
extern std::string HttpVersionToString(HttpVersion method);
extern std::string FileNameToMime(const std::string&name);

#define USE_BUILTIN(type,name,id,path)\
	extern "C"{\
		extern char bin_##name##_start[];\
		extern uint64_t bin_##name##_size;\
	};\
	cdecl_attr_used HttpStaticHandler builtin_handler_##id(path,type,bin_##name##_start,bin_##name##_size);

#define USE_BUILTIN_LIST(name,id,path)\
	extern "C"{\
		extern struct file_item name##_file_list[];\
	};\
	cdecl_attr_used HttpStaticListHandler builtin_handler_##id(path,name##_file_list);

namespace Exceptions{
	class HttpError:public RuntimeError{
		public:
			inline HttpError()=default;
			explicit HttpError(
				int code,const std::string&msg,
				std::source_location c=std::source_location::current()
			);
			virtual ~HttpError()=default;
			int code=MHD_HTTP_INTERNAL_SERVER_ERROR;
	};
}
#ifndef BARE_EXCEPTIONS
#define HttpError(code,...) Exceptions::HttpError(code,std::format(__VA_ARGS__))
#endif

#endif
