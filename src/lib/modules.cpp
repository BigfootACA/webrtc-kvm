/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<memory>
#include"external/kmod.h"

class kmod_context{
	public:
		inline kmod_context(){
			ctx=kmod.kmod_new(nullptr,nullptr);
		}
		inline ~kmod_context(){
			if(ctx)kmod.kmod_unref(ctx);
		}
		static inline std::shared_ptr<kmod_context>get(){
			if(!instance)instance=std::make_shared<kmod_context>();
			return instance;
		}
		kmod_ctx*ctx=nullptr;
		KMOD kmod;
		static std::shared_ptr<kmod_context>instance;
};

std::shared_ptr<kmod_context>kmod_context::instance=nullptr;

bool LoadModule(const std::string&name){
	int ret;
	bool success=false;
	kmod_list*list=nullptr,*l;
	std::shared_ptr<kmod_context>ctx;
	try{
		ctx=kmod_context::get();
		if(!ctx)throw InvalidArgument("bad context");
	}catch(...){return false;}
	ret=ctx->kmod.kmod_module_new_from_lookup(ctx->ctx,name.c_str(),&list);
	if(ret<0)return false;
	kmod_list_foreach(&ctx->kmod,l,list){
		auto mod=ctx->kmod.kmod_module_get_module(l);
		if(!mod)continue;
		ret=ctx->kmod.kmod_module_insert_module(mod,0,nullptr);
		if(ret>=0)success=true;
		ctx->kmod.kmod_module_unref(mod);
	}
	ctx->kmod.kmod_module_unref_list(list);
	return success;
}
