/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"gadget.h"

std::string MouseModeToString(mouse_mode mode){
	switch(mode){
		case MOUSE_ABSOLUTE:return "absolute";
		case MOUSE_RELATIVE:return "relative";
		case MOUSE_TOUCHSCREEN:return "touchscreen";
		default:return "unknown";
	}
}

mouse_mode StringToMouseMode(const std::string&mode){
	if(mode=="absolute")return MOUSE_ABSOLUTE;
	if(mode=="relative")return MOUSE_RELATIVE;
	if(mode=="touchscreen")return MOUSE_TOUCHSCREEN;
	return MOUSE_UNKNOWN;
}
