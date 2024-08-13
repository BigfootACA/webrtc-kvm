/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

static const std::string index_pages[]={
	"index.html",
	"index.htm",
};

MHD_Result HttpHandler::HandleConnection(
	void*cls,
	MHD_Connection*conn,
	const char*url,
	const char*method,
	const char*version,
	const char*data,
	size_t*data_size,
	void**con_cls
){
	auto ctx=(webrtc_kvm*)cls;
	HttpVersion val_version=HTTP_VERSION_UNKNOWN;
	HttpMethod val_method=HTTP_METHOD_UNKNOWN;
	if(!StringToHttpVersion(version,val_version))return MHD_NO;
	if(!StringToHttpMethod(method,val_method))return MHD_NO;
	HttpRequestContext context(ctx,conn);
	context.url=url,context.method=val_method,context.version=val_version;
	context.data=data,context.data_size=data_size,context.con_cls=con_cls;
	context.auth=ctx->http.auth.auth;
	return context.CatchRun([](auto ctx)->MHD_Result {
		if(auto handler=HttpHandler::GetHandler(ctx->url))
			return handler->Process(ctx);
		if(ctx->url.ends_with("/"))for(const auto&p:index_pages)
			if(auto handler=HttpHandler::GetHandler(ctx->url+p))
				return handler->Process(ctx);
		throw HttpError(MHD_HTTP_NOT_FOUND,"no handler to process");
	});
}
