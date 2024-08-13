/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"

void WebRTCEndpoint::OnStartStream(){
	Json::Value event;
	event["type"]="stream_started";
	SendEventAll(event);
	ReportSize();
	frames=0;
}

void WebRTCEndpoint::OnStopStream(){
	Json::Value event;
	event["type"]="stream_stopped";
	SendEventAll(event);
	frames=0;
}
