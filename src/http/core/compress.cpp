/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"lib/readable.h"
#include"compress/compress.h"

HttpCompress::HttpCompress(
	const std::string&name,
	const std::shared_ptr<Blob>blob,
	MHD_ResponseMemoryMode mode
):name(name),file(blob),mode(mode){

}

std::shared_ptr<Blob>HttpCompress::GenerateCompress(const std::string&type){
	if(std::find(skipped.begin(),skipped.end(),type)!=skipped.end())return nullptr;
	auto method=Compressor::ByName(type);
	if(!method)return nullptr;
	auto len=file->GetLength();
	try{
		size_t pos=0;
		auto compressed=method->Compress(*file.get(),pos);
		if(!compressed)throw RuntimeError("no compressed data");
		if(pos!=len)throw RuntimeError(
			"compress size of {} mismatch: {} != {}",
			name,pos,len
		);
		auto comp_size=compressed->GetLength();
		if(comp_size==0)throw RuntimeError(
			"compressed data of {} is empty",name
		);
		if(comp_size>len)throw RuntimeError(
			"compressed data larger than original data: {} > {}",
			SizeStringFloat(comp_size),SizeStringFloat(len)
		);
		log_info(
			"compressed {} with {} size {} to {} ({}%)",
			name,type,SizeStringFloat(len),SizeStringFloat(comp_size),comp_size*100/len
		);
		cache[type]=compressed;
		return compressed;
	}catch(std::exception&exc){
		log_warn("compress {} with {} failed: {}",name,type,exc.what());
		skipped.push_back(type);
		return nullptr;
	}
}

MHD_Result HttpCompress::TryCompress(const std::string&mime,HttpRequestContext*ctx){
	auto size=file->GetLength();
	if(size<0x400||size>=0x800000)return MHD_SKIP;
	if(ctx->req_headers.Have(MHD_HTTP_HEADER_CONTENT_ENCODING))return MHD_SKIP;
	auto accepts=ctx->req_headers.GetList(MHD_HTTP_HEADER_ACCEPT_ENCODING);
	if(accepts.empty())return MHD_SKIP;
	std::reverse(accepts.begin(),accepts.end());
	for(auto accept:accepts){
		auto item=cache.find(accept);
		std::shared_ptr<Blob>result;
		if(item==cache.end()){
			result=GenerateCompress(accept);
			if(!result)continue;
		}else result=item->second;
		ctx->res_headers.Add(MHD_HTTP_HEADER_CONTENT_ENCODING,accept);
		return ctx->HandleData(MHD_HTTP_OK,mime,result->ToPtr(),result->GetLength(),mode);
	}
	return MHD_SKIP;
}
