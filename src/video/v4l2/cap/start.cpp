/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_cap.h"

void V4L2Capture::OnStartStream(){
	V4L2Device::OnStartStream();
	EnablePoll();
}

void V4L2Capture::OnStopStream(){
	DisablePoll();
	V4L2Device::OnStopStream();
}
