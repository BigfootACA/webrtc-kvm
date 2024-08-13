/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<bzlib.h>
#include"compress.h"
#include"lib/exception.h"

class Bzip2CompressContext:public CompressContext{
	public:
		inline ~Bzip2CompressContext(){Deinit();}
		bz_stream zs{};
		int mode=0;
};

class Bzip2Compressor:public Compressor{
	public:
		inline Bzip2Compressor(){RegisterSelf();};
		inline ~Bzip2Compressor()=default;
		inline std::vector<std::string>GetMimes()final{return {"application/x-bzip2"};}
		inline std::vector<std::string>GetExts()final{return {"bz2","bzip2"};}
		inline std::vector<std::string>GetNames()final{return {"bzip2"};}
		inline std::string GetType()final{return "bzip2";}
		bool IsCompressed(const Blob&data)final;
		std::shared_ptr<CompressContext>CompressInit()final;
		std::shared_ptr<CompressContext>DecompressInit()final;
		void CompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos)final;
		void DecompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos)final;
};

std::shared_ptr<CompressContext>Bzip2Compressor::DecompressInit(){
	auto ctx=std::make_shared<Bzip2CompressContext>();
	if((BZ2_bzDecompressInit(&ctx->zs,0,0))!=BZ_OK)
		throw RuntimeError("zlib inflate init failed");
	ctx->data=std::make_shared<Blob>(COMPRESS_STEP_SIZE);
	ctx->deinit=[ctx](){BZ2_bzDecompressEnd(&ctx->zs);};
	ctx->mode=2;
	return ctx;
}

std::shared_ptr<CompressContext>Bzip2Compressor::CompressInit(){
	auto ctx=std::make_shared<Bzip2CompressContext>();
	if((BZ2_bzCompressInit(&ctx->zs,0,0,0))!=BZ_OK)
		throw RuntimeError("zlib inflate init failed");
	ctx->data=std::make_shared<Blob>(COMPRESS_STEP_SIZE);
	ctx->deinit=[ctx](){BZ2_bzCompressEnd(&ctx->zs);};
	ctx->mode=1;
	return ctx;
}

void Bzip2Compressor::DecompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos){
	int status=0;
	if(input.IsEmpty())throw InvalidArgument("bad input");
	auto g=std::dynamic_pointer_cast<Bzip2CompressContext>(ctx);
	if(!g||g->mode!=2)throw InvalidArgument("bad context");
	g->zs.next_in=input.ToCharPtr();
	g->zs.avail_in=input.GetLength();
	do{
		size_t avail=g->data->GetLength()-g->len;
		g->zs.avail_out=avail;
		g->zs.next_out=g->data->ToCharPtr()+g->len;
		if(avail<=0)status=BZ_OUTBUFF_FULL;
		else status=BZ2_bzDecompress(&g->zs);
		if(status==BZ_OUTBUFF_FULL){
			g->data->AddSize(COMPRESS_STEP_SIZE);
			continue;
		}
		if(status<0)throw RuntimeError("bzip2 decompress failed: {}",status);
		size_t processed=avail-g->zs.avail_out;
		g->len+=processed;
	}while(status!=BZ_STREAM_END);
	pos=input.GetLength()-g->zs.avail_in;
}

void Bzip2Compressor::CompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos){
	int status=0;
	if(input.IsEmpty())throw InvalidArgument("bad input");
	auto g=std::dynamic_pointer_cast<Bzip2CompressContext>(ctx);
	if(!g||g->mode!=1)throw InvalidArgument("bad context");
	g->zs.next_in=input.ToCharPtr();
	g->zs.avail_in=input.GetLength();
	do{
		size_t avail=g->data->GetLength()-g->len;
		g->zs.avail_out=avail;
		g->zs.next_out=g->data->ToCharPtr()+g->len;
		if(avail<=0)status=BZ_OUTBUFF_FULL;
		else status=BZ2_bzCompress(&g->zs,BZ_RUN);
		if(status==BZ_OUTBUFF_FULL){
			g->data->AddSize(COMPRESS_STEP_SIZE);
			continue;
		}
		if(status<0)throw RuntimeError("bzip2 compress failed: {}",status);
		size_t processed=avail-g->zs.avail_out;
		g->len+=processed;
	}while(status!=BZ_STREAM_END);
	pos=input.GetLength()-g->zs.avail_in;
}
