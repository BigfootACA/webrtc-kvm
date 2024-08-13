/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

MHD_Result HttpPOSTHandler::Handle(HttpRequestContext*ctx){
	MHD_Result res;
	auto pd=(post_data*)*ctx->con_cls;
	if(ctx->method==HTTP_METHOD_POST&&(res=FetchPost(ctx))!=MHD_SKIP)return res;
	HttpPOSTRequestContext context(ctx,pd);
	return HandlePOST(&context);
}

MHD_Result HttpPOSTHandler::FetchPost(HttpRequestContext*ctx){
	const size_t ps=sizeof(post_data);
	size_t size=HTTP_POST_STEP;
	auto pd=(post_data*)*ctx->con_cls;
	if(!pd){
		pd=(post_data*)malloc(ps+size);
		if(unlikely(!pd))return MHD_NO;
		memset(pd,0,ps);
		pd->size=size,*ctx->con_cls=pd;
		return MHD_YES;
	}else if(ctx->data&&ctx->data_size){
		size=pd->len+(*ctx->data_size)+1;
		if(size>=HTTP_POST_MAX)pd->overflow=true;
		if(likely(!pd->overflow)){
			if(size>=pd->size){
				size=round_up(size,HTTP_POST_STEP);
				pd=(post_data*)realloc(*ctx->con_cls,ps+size);
				if(!pd)return MHD_NO;
				*ctx->con_cls=pd,pd->size=size;
			}
			memcpy(&pd->payload[pd->len],ctx->data,*ctx->data_size);
			pd->len+=*ctx->data_size;
			pd->payload[pd->len]=0;
		}
		*ctx->data_size=0;
		return MHD_YES;
	}
	if(pd->overflow)throw HttpError(MHD_HTTP_CONTENT_TOO_LARGE,"post data too large");
	return MHD_SKIP;
}

HttpPOSTRequestContext::HttpPOSTRequestContext(
	HttpRequestContext*p,post_data*pd
):HttpRequestContext(p->ctx,p->conn),post(pd){
	url=p->url,method=p->method,version=p->version;
	data=p->data,data_size=p->data_size;
	con_cls=p->con_cls,auth=p->auth;
}
