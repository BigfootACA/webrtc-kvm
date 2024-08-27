/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_H
#define GADGET_H
#include"webrtc_kvm.h"

enum mouse_mode{
	MOUSE_UNKNOWN     = 0,
	MOUSE_ABSOLUTE    = 1,
	MOUSE_RELATIVE    = 2,
	MOUSE_TOUCHSCREEN = 3,
};

extern std::string MouseModeToString(mouse_mode mode);
extern mouse_mode StringToMouseMode(const std::string&mode);

#endif
