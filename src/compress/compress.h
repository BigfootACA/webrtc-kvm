/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef COMPRESS_H
#define COMPRESS_H
#include<vector>
#include<string>
#include<memory>
#include<functional>
#include"algo/blob.h"

#define COMPRESS_STEP_SIZE 0x1000

class CompressContext{
	public:
		virtual ~CompressContext()=default;
		inline void Deinit(){if(init&&deinit)deinit(),init=false;}
		std::function<void()>deinit=nullptr;
		bool init=true;
		std::shared_ptr<Blob>data;
		size_t len=0;
};

class Compressor{
	public:
		virtual ~Compressor(){UnregisterSelf();}
		virtual bool IsMimeSupported(const std::string&mime);
		virtual bool IsNameSupported(const std::string&name);
		virtual bool IsExtSupported(const std::string&ext);
		virtual std::string GetName();
		virtual std::string GetMime();
		virtual std::string GetExt();
		virtual std::vector<std::string>GetMimes()=0;
		virtual std::vector<std::string>GetExts()=0;
		virtual std::vector<std::string>GetNames()=0;
		virtual std::string GetType()=0;
		virtual bool IsCompressed(const Blob&data)=0;
		virtual std::shared_ptr<CompressContext>CompressInit();
		virtual void CompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos)=0;
		virtual std::shared_ptr<Blob>CompressEnd(std::shared_ptr<CompressContext>ctx);
		virtual std::shared_ptr<CompressContext>DecompressInit();
		virtual void DecompressFeed(std::shared_ptr<CompressContext>ctx,const Blob&input,size_t&pos)=0;
		virtual std::shared_ptr<Blob>DecompressEnd(std::shared_ptr<CompressContext>ctx);
		std::shared_ptr<Blob>Compress(const Blob&input,size_t&pos);
		std::shared_ptr<Blob>Decompress(const Blob&input,size_t&pos);
		static Compressor*LoadPlugin(const std::string&name);
		static Compressor*ByType(const std::string&type);
		static Compressor*ByName(const std::string&name);
		static Compressor*ByExt(const std::string&ext);
		static Compressor*ByMime(const std::string&mime);
		static Compressor*ByFileName(const std::string&filename);
		static Compressor*ByContent(const Blob&data);
		static void Register(Compressor*comp);
		static void Unregister(Compressor*comp);
	protected:
		inline void RegisterSelf(){Register(this);}
		inline void UnregisterSelf(){Unregister(this);}
		static std::vector<Compressor*>compressor;
};

#define DECL_COMPRESS_BUILTIN(cls,name) cdecl_attr_used cls compressor_##name;
#define DECL_COMPRESS(cls,name) \
	DECL_COMPRESS_BUILTIN(cls,name) \
	extern "C" Compressor*webrtc_plugin_compress_get_##name(){\
		return &compressor_##name;\
	}
#endif
