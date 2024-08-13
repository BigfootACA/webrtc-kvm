/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"abstract/async-runner.h"

class HttpExitHandler:public HttpRestfulHandler{
	public:
		inline HttpExitHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/exit";}
		[[nodiscard]] bool NeedAuth()const final{return true;}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpExitHandler http_exit_handler;

int HttpExitHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	AsyncRunner::DefaultRunLater([kvm=ctx->ctx](auto) {
		SetRunning(kvm,false);
	});
	return MHD_HTTP_OK;
}
