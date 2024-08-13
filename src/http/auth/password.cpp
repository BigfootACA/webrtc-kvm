/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

void HttpAuthorization::AuthPassword(const AuthRequest&req){
	auto&pwd=ctx->http.auth.pwd;
	auto i=pwd.users.find(req.username);
	if(i==pwd.users.end())
		throw HttpError(MHD_HTTP_UNAUTHORIZED,"username not found");
	if(i->second!=req.password)
		throw HttpError(MHD_HTTP_UNAUTHORIZED,"password mismatch");
}
