/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"gadget/gadget_ctx.h"

class HttpInputSetMouseModeHandler:public HttpRestfulHandler{
	public:
		inline HttpInputSetMouseModeHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/input/set_mouse_mode";}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpInputSetMouseModeHandler http_input_set_mouse_mode_handler;

int HttpInputSetMouseModeHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	auto mode=MOUSE_UNKNOWN;
	if(auto v=ctx->request["mode"])
		mode=StringToMouseMode(v.asString());
	if(mode==MOUSE_UNKNOWN)
		throw HttpError(MHD_HTTP_BAD_REQUEST,"invalid mouse mode");
	if(!ctx->ctx||!ctx->ctx->usb.gadget)
		throw HttpError(MHD_HTTP_TOO_EARLY,"gadget context is not ready");
	GadgetSetMouseMode(ctx->ctx,mode);
	ctx->response["mode"]=MouseModeToString(mode);
	return MHD_HTTP_OK;
}
