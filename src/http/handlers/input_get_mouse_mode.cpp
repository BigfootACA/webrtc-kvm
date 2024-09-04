/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"gadget/gadget_ctx.h"

class HttpInputGetMouseModeHandler:public HttpRestfulHandler{
	public:
		inline HttpInputGetMouseModeHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/input/get_mouse_mode";}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpInputGetMouseModeHandler http_input_get_mouse_mode_handler;

int HttpInputGetMouseModeHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	auto mode=MOUSE_UNKNOWN;
	if(ctx->ctx&&ctx->ctx->usb.gadget)
		mode=ctx->ctx->usb.gadget->last_mode;
	ctx->response["mode"]=MouseModeToString(mode);
	return MHD_HTTP_OK;
}
