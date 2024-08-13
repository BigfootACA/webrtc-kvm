/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

HttpRestfulRequestContext::HttpRestfulRequestContext(HttpPOSTRequestContext*p)
	:HttpPOSTRequestContext(p,p->post){}

MHD_Result HttpRestfulHandler::HandlePOST(HttpPOSTRequestContext*ctx){
	Json::Reader reader;
	HttpRestfulRequestContext context(ctx);
	if(ctx->post&&!reader.parse(ctx->content(),context.request))
		throw HttpError(MHD_HTTP_BAD_REQUEST,"bad json: {}",reader.getFormattedErrorMessages());
	int code=HandleRestful(&context);
	return context.HandleJSON(code,context.response);
}
