/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"
#include"string/strings.h"

void WebRTCEndpoint::OnBindInput(std::shared_ptr<StreamLink>link){
	if(fourcc==0&&link->fourcc!=0)fourcc=link->fourcc;
	if(fourcc!=0&&link->fourcc==0)link->fourcc=fourcc;
	if(fourcc==0&&link->fourcc==0)throw InvalidArgument("no fourcc in link");
	if(fourcc!=link->fourcc)throw InvalidArgument(
		"fourcc mismatch: {} != {}",
		FourccToString(fourcc),
		FourccToString(link->fourcc)
	);
}
