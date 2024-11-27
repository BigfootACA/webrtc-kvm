/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"compress.h"
#include"lib/exception.h"

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
