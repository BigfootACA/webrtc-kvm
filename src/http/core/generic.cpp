/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"lib/readable.h"

struct HttpHandlers{
	std::map<std::string,HttpHandler*>handlers;
};

static HttpHandlers*handlers=nullptr;

void HttpHandler::RegisterSelf(){
	auto path=GetPath();
	if(!handlers)handlers=new HttpHandlers;
	if(handlers->handlers.find(path)!=handlers->handlers.end())
		throw RuntimeError("handler {} already exists",path);
	handlers->handlers[path]=this;
}

HttpHandler*HttpHandler::GetHandler(const std::string&url){
	return handlers->handlers[url];
}

MHD_Result HttpRequestContext::HandleJSON(int code,Json::Value&val){
	Json::FastWriter writer;
	val["code"]=code;
	val["success"]=(bool)(code<400||code>599);
	auto str=writer.write(val);
	return HandleString(code,"application/json",str);
}

MHD_Result HttpRequestContext::HandleString(int code,const std::string&mime,const std::string&val){
	MHD_Result ret;
	HttpCompress comp(url,std::make_shared<Blob>(val),MHD_RESPMEM_MUST_COPY);
	if((ret=comp.TryCompress(mime,this))!=MHD_SKIP)return ret;
	return HandleData(code,mime.c_str(),(void*)val.c_str(),val.length(),MHD_RESPMEM_MUST_COPY);
}

MHD_Result HttpRequestContext::HandleData(int code,const std::string&mime,void*body,size_t len,MHD_ResponseMemoryMode mode){
	MHD_Response*response=MHD_create_response_from_buffer(len,body,mode);
	if(!response)return MHD_NO;
	log_dbg(
		"http response {} {} {} with code {} type {} size {}",
		HttpMethodToString(method),url,
		HttpVersionToString(version),code,
		mime.empty()?"(none)":mime,SizeStringFloat(len)
	);
	if(!mime.empty())res_headers.Ensure(MHD_HTTP_HEADER_CONTENT_TYPE,mime);
	res_headers.FillToResponse(response);
	MHD_Result res=MHD_queue_response(conn,code,response);
	MHD_destroy_response(response);
	return res;
}

HttpRequestContext::HttpRequestContext(webrtc_kvm*ctx,MHD_Connection*conn):ctx(ctx),conn(conn){
	req_headers.FillFromConnection(conn);
	res_headers.Add(MHD_HTTP_HEADER_DATE,HttpHeaders::TimeToString());
	res_headers.Add(MHD_HTTP_HEADER_SERVER,"WebRTC KVM / MicroHTTPD");
	res_headers.Add(MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_CREDENTIALS,"true");
	res_headers.Add(MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_METHODS,"GET,POST,HEAD");
	res_headers.Add(MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN,"*");
	res_headers.Add(MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_HEADERS,"*");
	res_headers.Add(MHD_HTTP_HEADER_ACCESS_CONTROL_EXPOSE_HEADERS,"ETag,Content-Length,Content-Type,Authorization");
}
