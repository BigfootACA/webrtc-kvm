/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"

void WebRTCInstance::ProcessVideoStream(const Blob&data){
	if(unlikely(state!=rtc::PeerConnection::State::Connected))
		throw InvalidArgument("connection is not ready for process video stream");
	auto cfg=video.sr->rtpConfig;
	cfg->timestamp+=cfg->secondsToTimestamp(1.0f/double(kvm->video.fps));
	auto report=cfg->timestamp-video.sr->lastReportedTimestamp();
	if(cfg->timestampToSeconds(report)>1)video.sr->setNeedsToReport();
	video.track->send(data.ToBytes(),data.GetLength());
}

void WebRTCEndpoint::OnProcessInput(StreamBuffer*buffer){
	if(unlikely(buffer->type!=BUFFER_POINTER))
		throw InvalidArgument("buffer is not pointer");
	if(unlikely(frames==0))ReportSize();
	frames++;
	for(uint32_t i=0;i<buffer->planes_cnt;i++){
		Blob data(buffer->planes[i].ptr,buffer->planes[i].used);
		for(const auto&ins:streaming)ins->ProcessVideoStream(data);
	}
}
