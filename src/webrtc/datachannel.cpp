/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"

void WebRTCInstance::OnDataChannel(std::shared_ptr<rtc::DataChannel>ch){
	log_info("webrtc {} got new data channel {}",uuid.ToString(),ch->label());
}
