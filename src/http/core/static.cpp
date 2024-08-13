/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"lib/readable.h"

HttpStaticHandler::HttpStaticHandler(
	const std::string&path,
	const std::string&mime,
	const std::shared_ptr<Blob>blob
):path(path),mime(mime),file(blob),modified(time(nullptr)){
	RegisterSelf();
	comp=std::make_shared<HttpCompress>(path,blob,MHD_RESPMEM_PERSISTENT);
}

HttpStaticHandler::HttpStaticHandler(
	const std::string&path,
	const std::string&mime,
	const void*data,
	size_t len
):HttpStaticHandler(path,mime,std::make_shared<Blob>((void*)data,len)){}

HttpStaticHandler::HttpStaticHandler(
	const std::string&path,
	const void*data,
	size_t len
):HttpStaticHandler(path,FileNameToMime(path),data,len){}

HttpStaticHandler::HttpStaticHandler(
	const std::string&path,
	const std::shared_ptr<Blob>blob
):HttpStaticHandler(path,
FileNameToMime(path),blob){}

MHD_Result HttpStaticHandler::TryModified(HttpRequestContext*ctx){
	ctx->res_headers.Add(
		MHD_HTTP_HEADER_LAST_MODIFIED,
		HttpHeaders::TimeToString(modified)
	);
	auto since=ctx->req_headers.GetOne(MHD_HTTP_HEADER_IF_MODIFIED_SINCE);
	if(since.empty())return MHD_SKIP;
	auto ts=HttpHeaders::StringToTime(since);
	if(ts<modified)return HandleUnmodified(ctx);
	return MHD_SKIP;
}

MHD_Result HttpStaticHandler::Handle(HttpRequestContext*ctx){
	MHD_Result ret;
	if((ret=TryModified(ctx))!=MHD_SKIP)return ret;
	if((ret=comp->TryCompress(mime,ctx))!=MHD_SKIP)return ret;
	return ctx->HandleData(MHD_HTTP_OK,mime,file->ToPtr(),file->GetLength(),MHD_RESPMEM_PERSISTENT);
}

MHD_Result HttpStaticHandler::HandleUnmodified(HttpRequestContext*ctx){
	static const char*empty="";
	return ctx->HandleData(
		MHD_HTTP_NOT_MODIFIED,mime.c_str(),
		(void*)empty,0,MHD_RESPMEM_PERSISTENT
	);
}

HttpStaticListHandler::HttpStaticListHandler(const std::string&path,const file_item*items){
	if(!items)throw InvalidArgument("bad items");
	for(size_t i=0;items[i].name;i++){
		std::string name=items[i].name;
		if(items[i].len>=SIZE_MAX)throw InvalidArgument(
			"file {} too large: {}",
			name,SizeStringFloat(items[i].len)
		);
		std::string file_path;
		if(!path.empty())file_path+=path;
		if(!path.ends_with("/")&&!name.starts_with("/"))file_path+="/";
		file_path+=name;
		auto handler=std::make_shared<HttpStaticHandler>(
			file_path,(void*)items[i].data,items[i].len
		);
		handlers.push_back(handler);
	}
}
