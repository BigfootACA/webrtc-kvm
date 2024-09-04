/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"video.h"

void VideoProcess::onFrame(rtc::binary b,rtc::FrameInfo f){
	int ret;
	std::unique_lock<std::mutex>l(lock);
	if(unlikely(b.empty()))return;
	if(unlikely(!codec||!ctx))return;
	screen->current.packets+=1;
	screen->current.bytes+=b.size();
	try{
		memset(&packet,0,sizeof(packet));
		packet.data=(uint8_t*)b.data();
		packet.size=b.size();
		packet.pts=f.timestamp;
		packet.dts=f.timestamp;
		if(unlikely(!ctx))throw InvalidArgument("no avcodec context");
		if(unlikely((ret=avcodec_send_packet(ctx,&packet))<0))
			throw ErrnoExceptionWith(AVUNERROR(ret),"avcodec send packet failed");
		while(receiveFrame());
	}catch(std::exception&exc){
		log_warn("process decoded_frame failed: {}",exc.what());
	}
}
