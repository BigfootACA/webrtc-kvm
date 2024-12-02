/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<brotli/encode.h>
#include<brotli/decode.h>
#include"compress.h"
#include"defines.h"
#include"lib/exception.h"

class BrotliCompressContext:public CompressContext{
	public:
		inline ~BrotliCompressContext(){Deinit();}
		BrotliEncoderState*enc=nullptr;
		BrotliDecoderState*dec=nullptr;
};

class BrotliCompressor:public Compressor{
	public:
		inline BrotliCompressor(){RegisterSelf();};
		inline ~BrotliCompressor()=default;
		inline std::vector<std::string>GetMimes()final{return {"application/x-brotli"};}
		inline std::vector<std::string>GetExts()final{return {"br","brotli"};}
		inline std::vector<std::string>GetNames()final{return {"brotli","br"};}
		inline std::string GetType()final{return "brotli";}
		bool IsCompressed(const Blob&data)final;
		std::shared_ptr<CompressContext>CompressInit()final;
		std::shared_ptr<CompressContext>DecompressInit()final;
		void CompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos)final;
		void DecompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos)final;
		virtual std::shared_ptr<Blob>CompressEnd(std::shared_ptr<CompressContext>ctx)final;
};

DECL_COMPRESS(BrotliCompressor,brotli)

std::shared_ptr<CompressContext>BrotliCompressor::CompressInit(){
	auto ctx=std::make_shared<BrotliCompressContext>();
	if(!(ctx->enc=BrotliEncoderCreateInstance(nullptr,nullptr,nullptr)))
		throw RuntimeError("brotli compress context create failed");
	ctx->data=std::make_shared<Blob>(COMPRESS_STEP_SIZE);
	ctx->deinit=[ctx](){if(ctx->enc)BrotliEncoderDestroyInstance(ctx->enc);};
	return ctx;
}

std::shared_ptr<CompressContext>BrotliCompressor::DecompressInit(){
	auto ctx=std::make_shared<BrotliCompressContext>();
	if(!(ctx->dec=BrotliDecoderCreateInstance(nullptr,nullptr,nullptr)))
		throw RuntimeError("brotli decompress context create failed");
	ctx->data=std::make_shared<Blob>(COMPRESS_STEP_SIZE);
	ctx->deinit=[ctx](){if(ctx->dec)BrotliDecoderDestroyInstance(ctx->dec);};
	return ctx;
}

void BrotliCompressor::CompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos){
	if(input.IsEmpty())throw InvalidArgument("bad input");
	auto g=std::dynamic_pointer_cast<BrotliCompressContext>(ctx);
	if(!g||!g->enc)throw InvalidArgument("bad context");
	const uint8_t*ibuf=input.ToUCharPtr();
	size_t ilen=input.GetLength();
	auto needed=BrotliEncoderMaxCompressedSize(ilen)+ctx->len;
	if(needed>ctx->data->GetLength())ctx->data->Resize(needed);
	do{
		size_t out=0;
		auto obuf=ctx->data->ToUCharPtr()+ctx->len;
		auto olen=ctx->data->GetLength()-ctx->len;
		auto ret=BrotliEncoderCompressStream(
			g->enc,BROTLI_OPERATION_PROCESS,
			&ilen,&ibuf,&olen,&obuf,&out
		);
		if(!ret)throw RuntimeError("brotli compress failed");
		ctx->len+=out;
	}while(ilen>0);
	pos=input.GetLength();
}

void BrotliCompressor::DecompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos){
	if(input.IsEmpty())throw InvalidArgument("bad input");
	auto g=std::dynamic_pointer_cast<BrotliCompressContext>(ctx);
	if(!g||!g->dec)throw InvalidArgument("bad context");
	const uint8_t*ibuf=input.ToUCharPtr();
	size_t ilen=input.GetLength();
	bool process=true;
	while(process){
		size_t out=0;
		auto obuf=ctx->data->ToUCharPtr()+ctx->len;
		auto olen=ctx->data->GetLength()-ctx->len;
		auto ret=BrotliDecoderDecompressStream(g->dec,&ilen,&ibuf,&olen,&obuf,&out);
		ctx->len+=out;
		switch(ret){
			case BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT:
				if(ilen>0)continue;//fallthrough
			case BROTLI_DECODER_RESULT_SUCCESS:
				process=false;
			break;
			case BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT:
				g->data->AddSize(COMPRESS_STEP_SIZE);
			break;
			case BROTLI_DECODER_RESULT_ERROR:{
				auto code=BrotliDecoderGetErrorCode(g->dec);
				auto reason=BrotliDecoderErrorString(code);
				throw RuntimeError("brotli decompress failed: {}",reason);
			}break;
			default:throw RuntimeError("brotli decompress failed");
		}
	}
	pos=input.GetLength()-ilen;
}

std::shared_ptr<Blob>BrotliCompressor::CompressEnd(std::shared_ptr<CompressContext>ctx){
	auto g=std::dynamic_pointer_cast<BrotliCompressContext>(ctx);
	if(!g||!g->enc)throw InvalidArgument("bad context");
	size_t ilen=0,out=0;
	uint8_t ibuf[1];
	auto obuf=ctx->data->ToUCharPtr()+ctx->len;
	auto olen=ctx->data->GetLength()-ctx->len;
	auto ret=BrotliEncoderCompressStream(
		g->enc,BROTLI_OPERATION_FINISH,
		&ilen,(const uint8_t**)&ibuf,&olen,&obuf,&out
	);
	if(!ret)throw RuntimeError("brotli compress finish failed");
	ctx->len+=out;
	ctx->Deinit();
	ctx->data->SetLength(ctx->len);
	return ctx->data;
}

bool BrotliCompressor::IsCompressed(const Blob&data){
	return false;
}
