/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

void HttpAuthorization::AuthPAM(const AuthRequest&req){
	throw RuntimeError("PAM library is not linked");
}
