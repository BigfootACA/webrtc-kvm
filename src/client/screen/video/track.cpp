/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"video.h"

void WebRTCScreen::onVideoTrack(std::shared_ptr<rtc::Track>track){
	log_info("initialized video track {}",track->mid());
	video_process=std::make_shared<VideoProcess>(this,track);
	video_process->setSize(video->width(),video->height());
}

void VideoProcess::onClosed(){
	std::unique_lock<std::mutex>l(lock);
	log_info("video track closed");
	if(scaled_frame){
		initScaledFrame(scaled);
		memset(scaled_buffer,0,scaled_buffer_size);
		commitFrame(scaled_frame);
	}
	destroyAVCodec();
	destroyScaler();
}

void VideoProcess::onError(std::string err){
	log_warn("video track error: {}",err);
}
