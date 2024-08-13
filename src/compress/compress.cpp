/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<algorithm>
#include"compress.h"
#include"lib/exception.h"

std::vector<Compressor*>Compressor::compressor;

std::string Compressor::GetName(){
	auto names=GetNames();
	return names.empty()?"":names[0];
}

std::string Compressor::GetMime(){
	auto mimes=GetMimes();
	return mimes.empty()?"":mimes[0];
}

std::string Compressor::GetExt(){
	auto exts=GetExts();
	return exts.empty()?"":exts[0];
}

bool Compressor::IsNameSupported(const std::string&name){
	auto names=GetNames();
	return std::find(names.begin(),names.end(),name)!=names.end();
}

bool Compressor::IsMimeSupported(const std::string&mime){
	auto mimes=GetMimes();
	return std::find(mimes.begin(),mimes.end(),mime)!=mimes.end();
}

bool Compressor::IsExtSupported(const std::string&ext){
	auto exts=GetExts();
	return std::find(exts.begin(),exts.end(),ext)!=exts.end();
}

Compressor*Compressor::ByType(const std::string&type){
	for(auto c:compressor)if(c->GetType()==type)return c;
	return nullptr;
}

Compressor*Compressor::ByName(const std::string&name){
	for(auto c:compressor)if(c->IsNameSupported(name))return c;
	return nullptr;
}

Compressor*Compressor::ByExt(const std::string&ext){
	for(auto c:compressor)if(c->IsExtSupported(ext))return c;
	return nullptr;
}

Compressor*Compressor::ByMime(const std::string&mime){
	for(auto c:compressor)if(c->IsMimeSupported(mime))return c;
	return nullptr;
}

Compressor*Compressor::ByFileName(const std::string&filename){
	auto dot=filename.rfind('.');
	if(dot==std::string::npos)return nullptr;
	return ByExt(filename.substr(dot+1));
}

Compressor*Compressor::ByContent(const Blob&data){
	for(auto c:compressor)if(c->IsCompressed(data))return c;
	return nullptr;
}

void Compressor::Register(Compressor*comp){
	auto type=comp->GetType();
	if(type.empty())throw InvalidArgument("bad compressor type");
	if(ByType(type))throw InvalidArgument("compressor already exists");
	compressor.push_back(comp);
}

void Compressor::Unregister(Compressor*comp){
	auto idx=std::find(compressor.begin(),compressor.end(),comp);
	if(idx==compressor.end())return;
	compressor.erase(idx);
}

std::shared_ptr<Blob>Compressor::Compress(const Blob&input,size_t&pos){
	auto ctx=CompressInit();
	CompressFeed(ctx,input,pos);
	return CompressEnd(ctx);
}

std::shared_ptr<Blob>Compressor::Decompress(const Blob&input,size_t&pos){
	auto ctx=DecompressInit();
	DecompressFeed(ctx,input,pos);
	return DecompressEnd(ctx);
}

std::shared_ptr<Blob>Compressor::DecompressEnd(std::shared_ptr<CompressContext>ctx){
	if(!ctx)throw InvalidArgument("bad context");
	ctx->Deinit();
	ctx->data->SetLength(ctx->len);
	return ctx->data;
}

std::shared_ptr<Blob>Compressor::CompressEnd(std::shared_ptr<CompressContext>ctx){
	if(!ctx)throw InvalidArgument("bad context");
	ctx->Deinit();
	ctx->data->SetLength(ctx->len);
	return ctx->data;
}

std::shared_ptr<CompressContext>Compressor::CompressInit(){
	auto ctx=std::make_shared<CompressContext>();
	ctx->data=std::make_shared<Blob>(COMPRESS_STEP_SIZE);
	return ctx;
}

std::shared_ptr<CompressContext>Compressor::DecompressInit(){
	auto ctx=std::make_shared<CompressContext>();
	ctx->data=std::make_shared<Blob>(COMPRESS_STEP_SIZE);
	return ctx;
}
