/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef GADGET_CTX_H
#define GADGET_CTX_H
#include"core/gadget.h"
#include"core/gadget_config.h"
#include"functions/gadget_hid.h"
#include"gadget.h"

struct gadget_ctx{
	mouse_mode last_mode;
	std::shared_ptr<Gadget>gadget;
	std::shared_ptr<GadgetConfig>config;
	std::shared_ptr<GadgetHID>keyboard;
	std::shared_ptr<GadgetHID>mouse;
};

extern void InitializeGadget(webrtc_kvm*ctx);
extern void GadgetSetMouseMode(webrtc_kvm*ctx,mouse_mode mode);
extern void GadgetCloseInput(webrtc_kvm*ctx);
extern void GadgetOpenInput(webrtc_kvm*ctx);
#endif
