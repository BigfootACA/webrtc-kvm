/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef INPUT_CTX_H
#define INPUT_CTX_H
#include"hid.h"
#include"input.h"
#include"webrtc_kvm.h"

class InputContext{
	public:
		int hid_keyboard_fd=-1;
		int hid_absolute_fd=-1;
		int hid_relative_fd=-1;
		int hid_touchscreen_fd=-1;
		struct{
			HIDKeyboardReport keyboard;
			HIDAbsoluteReport absolute;
			HIDRelativeReport relative;
			HIDTouchscreenReport touchscreen;
		}report={};
		void Report(const InputEvent*ev);
		void ResetReport();
		void Close();
		InputContext(webrtc_kvm*ctx);
		webrtc_kvm*ctx;
};

#endif
