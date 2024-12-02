/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<zstd.h>
#include"compress.h"
#include"defines.h"
#include"lib/exception.h"

class ZStandardCompressContext:public CompressContext{
	public:
		inline ~ZStandardCompressContext(){Deinit();}
		ZSTD_CCtx*cctx=nullptr;
		ZSTD_DCtx*dctx=nullptr;
};

class ZStandardCompressor:public Compressor{
	public:
		inline ZStandardCompressor(){RegisterSelf();};
		inline ~ZStandardCompressor()=default;
		inline std::vector<std::string>GetMimes()final{return {"application/zstd"};}
		inline std::vector<std::string>GetExts()final{return {"zst","zstd"};}
		inline std::vector<std::string>GetNames()final{return {"zstd"};}
		inline std::string GetType()final{return "zstd";}
		bool IsCompressed(const Blob&data)final;
		std::shared_ptr<CompressContext>CompressInit()final;
		std::shared_ptr<CompressContext>DecompressInit()final;
		void CompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos)final;
		void DecompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos)final;
};

DECL_COMPRESS(ZStandardCompressor,zstd)

std::shared_ptr<CompressContext>ZStandardCompressor::CompressInit(){
	auto ctx=std::make_shared<ZStandardCompressContext>();
	if(!(ctx->cctx=ZSTD_createCCtx()))
		throw RuntimeError("zstd compress context create failed");
	ctx->data=std::make_shared<Blob>(COMPRESS_STEP_SIZE);
	ctx->deinit=[ctx](){if(ctx->cctx)ZSTD_freeCCtx(ctx->cctx);};
	return ctx;
}

std::shared_ptr<CompressContext>ZStandardCompressor::DecompressInit(){
	auto ctx=std::make_shared<ZStandardCompressContext>();
	if(!(ctx->dctx=ZSTD_createDCtx()))
		throw RuntimeError("zstd decompress context create failed");
	ctx->data=std::make_shared<Blob>(COMPRESS_STEP_SIZE);
	ctx->deinit=[ctx](){if(ctx->dctx)ZSTD_freeDCtx(ctx->dctx);};
	return ctx;
}

void ZStandardCompressor::CompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos){
	if(input.IsEmpty())throw InvalidArgument("bad input");
	auto g=std::dynamic_pointer_cast<ZStandardCompressContext>(ctx);
	if(!g||!g->cctx)throw InvalidArgument("bad context");
	auto ibuf=input.ToPtr();
	size_t ilen=input.GetLength();
	auto needed=ZSTD_compressBound(ilen)+g->len;
	if(needed>g->data->GetLength())g->data->Resize(needed);
	auto obuf=g->data->ToUCharPtr()+g->len;
	auto olen=g->data->GetLength()-g->len;
	auto ret=ZSTD_compress2(g->cctx,obuf,olen,ibuf,ilen);
	if(ZSTD_isError(ret))throw RuntimeError(
		"zstd compress failed: {}",
		ZSTD_getErrorName(ret)
	);
	g->len+=ret,pos=ilen;
}

void ZStandardCompressor::DecompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos){
	if(input.IsEmpty())throw InvalidArgument("bad input");
	auto g=std::dynamic_pointer_cast<ZStandardCompressContext>(ctx);
	if(!g||!g->dctx)throw InvalidArgument("bad context");
	auto ibuf=input.ToPtr();
	size_t ilen=input.GetLength();
	auto needed=ZSTD_getFrameContentSize(ibuf,ilen)+g->len;
	if(needed!=ZSTD_CONTENTSIZE_UNKNOWN&&needed!=ZSTD_CONTENTSIZE_ERROR)
		if(needed>g->data->GetLength())g->data->Resize(needed);
	auto isize=ZSTD_findFrameCompressedSize(ibuf,ilen);
	if(ZSTD_isError(isize))throw InvalidArgument(
		"bad zstd compressed data: {}",
		ZSTD_getErrorName(isize)
	);
	if(isize>ilen)throw InvalidArgument("bad context");
	auto obuf=g->data->ToUCharPtr()+g->len;
	auto olen=g->data->GetLength()-g->len;
	auto ret=ZSTD_decompressDCtx(g->dctx,obuf,olen,ibuf,isize);
	if(ZSTD_isError(ret))throw RuntimeError(
		"zstd decompress failed: {}",
		ZSTD_getErrorName(ret)
	);
	g->len+=ret,pos=isize;
}

bool ZStandardCompressor::IsCompressed(const Blob&data){
	auto ibuf=data.ToPtr();
	size_t ilen=data.GetLength();
	auto isize=ZSTD_findFrameCompressedSize(ibuf,ilen);
	return !ZSTD_isError(isize);
}
