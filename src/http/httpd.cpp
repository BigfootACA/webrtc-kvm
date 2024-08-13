/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"http.h"

static void handle_complete(
	void*cls,
	struct MHD_Connection*conn,
	void**req_cls,
	enum MHD_RequestTerminationCode toe
){
	if(*req_cls)free(*req_cls);
}

void InitializeHTTP(webrtc_kvm*ctx){
	ctx->http.auth.auth=new HttpAuthorization(ctx);
	ctx->http.httpd=MHD_start_daemon(
		MHD_USE_INTERNAL_POLLING_THREAD,
		ctx->http.port,
		nullptr,ctx,
		HttpHandler::HandleConnection,ctx,
		MHD_OPTION_NOTIFY_COMPLETED,handle_complete,ctx,
		MHD_OPTION_END
	);
	if(!ctx->http.httpd)throw RuntimeError("create http daemon failed");
	log_info("http server started at port {}",ctx->http.port);
}
