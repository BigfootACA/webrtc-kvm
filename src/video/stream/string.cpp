/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"

bool StringToPipeRole(const std::string&val,StreamPipeRole&role){
	if(val=="encoder")role=ROLE_ENCODER;
	else if(val=="decoder")role=ROLE_DECODER;
	else if(val=="converter")role=ROLE_CONVERTER;
	else return false;
	return true;
}
