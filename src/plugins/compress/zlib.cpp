/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<zlib.h>
#include"compress.h"
#include"defines.h"
#include"lib/exception.h"

class ZlibCompressContext:public CompressContext{
	public:
		inline ~ZlibCompressContext(){Deinit();}
		z_stream_s zs{};
		int mode=0;
};

class ZlibCompressor:public Compressor{
	public:
		bool IsCompressed(const Blob&data)final;
		std::shared_ptr<CompressContext>CompressInit()final;
		std::shared_ptr<CompressContext>DecompressInit()final;
		void CompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos)final;
		void DecompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos)final;
};

std::shared_ptr<CompressContext>ZlibCompressor::DecompressInit(){
	auto ctx=std::make_shared<ZlibCompressContext>();
	if((inflateInit2(&ctx->zs,-MAX_WBITS))!=Z_OK)
		throw RuntimeError("zlib inflate init failed");
	ctx->data=std::make_shared<Blob>(COMPRESS_STEP_SIZE);
	ctx->deinit=[ctx](){inflateEnd(&ctx->zs);};
	ctx->mode=2;
	return ctx;
}

std::shared_ptr<CompressContext>ZlibCompressor::CompressInit(){
	auto ctx=std::make_shared<ZlibCompressContext>();
	auto window=15;
	if(GetType()=="gzip")window+=16;
	if((deflateInit2(&ctx->zs,Z_DEFAULT_COMPRESSION,Z_DEFLATED,window,8,Z_DEFAULT_STRATEGY))!=Z_OK)
		throw RuntimeError("zlib deflate init failed");
	ctx->data=std::make_shared<Blob>(COMPRESS_STEP_SIZE);
	ctx->deinit=[ctx](){deflateEnd(&ctx->zs);};
	ctx->mode=1;
	return ctx;
}

void ZlibCompressor::DecompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos){
	int status=0;
	if(input.IsEmpty())throw InvalidArgument("bad input");
	auto g=std::dynamic_pointer_cast<ZlibCompressContext>(ctx);
	if(!g||g->mode!=2)throw InvalidArgument("bad context");
	g->zs.next_in=input.ToUCharPtr();
	g->zs.avail_in=input.GetLength();
	do{
		size_t avail=g->data->GetLength()-g->len;
		g->zs.avail_out=avail;
		g->zs.next_out=g->data->ToUCharPtr()+g->len;
		if(avail<=0)status=Z_BUF_ERROR;
		else status=inflate(&g->zs,Z_FINISH);
		if(status==Z_BUF_ERROR){
			g->data->AddSize(COMPRESS_STEP_SIZE);
			continue;
		}
		if(status<0)throw RuntimeError("zlib inflate failed: {}",status);
		size_t processed=avail-g->zs.avail_out;
		g->len+=processed;
	}while(status!=Z_STREAM_END);
	pos=input.GetLength()-g->zs.avail_in;
}

void ZlibCompressor::CompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos){
	int status=0;
	if(input.IsEmpty())throw InvalidArgument("bad input");
	auto g=std::dynamic_pointer_cast<ZlibCompressContext>(ctx);
	if(!g||g->mode!=1)throw InvalidArgument("bad context");
	g->zs.next_in=input.ToUCharPtr();
	g->zs.avail_in=input.GetLength();
	auto needed=deflateBound(&g->zs,g->zs.avail_in);
	if(needed>g->data->GetLength())g->data->Resize(needed);
	do{
		size_t avail=g->data->GetLength()-g->len;
		g->zs.avail_out=avail;
		g->zs.next_out=g->data->ToUCharPtr()+g->len;
		if(avail<=0)status=Z_BUF_ERROR;
		else status=deflate(&g->zs,Z_FINISH);
		if(status==Z_BUF_ERROR){
			g->data->AddSize(COMPRESS_STEP_SIZE);
			continue;
		}
		if(status<0)throw RuntimeError("zlib deflate failed: {}",status);
		size_t processed=avail-g->zs.avail_out;
		g->len+=processed;
	}while(status!=Z_STREAM_END);
	pos=input.GetLength()-g->zs.avail_in;
}

bool ZlibCompressor::IsCompressed(const Blob&data){
	return data.GetLength()>10&&data[0]==0x1f&&data[1]==0x8b&&data[2]==0x08;
}

class GzipCompressor:public ZlibCompressor{
	public:
		inline GzipCompressor(){RegisterSelf();};
		inline ~GzipCompressor()=default;
		inline std::vector<std::string>GetMimes()final{return {"application/gzip"};}
		inline std::vector<std::string>GetExts()final{return {"gz","gzip"};}
		inline std::vector<std::string>GetNames()final{return {"gzip"};}
		inline std::string GetType()final{return "gzip";}
};

DECL_COMPRESS(GzipCompressor,gzip)

class DeflateCompressor:public ZlibCompressor{
	public:
		inline DeflateCompressor(){RegisterSelf();};
		inline ~DeflateCompressor()=default;
		inline std::vector<std::string>GetMimes()final{return {"application/zlib"};}
		inline std::vector<std::string>GetExts()final{return {"zlib"};}
		inline std::vector<std::string>GetNames()final{return {"deflate"};}
		inline std::string GetType()final{return "deflate";}
};

DECL_COMPRESS(DeflateCompressor,deflate)
