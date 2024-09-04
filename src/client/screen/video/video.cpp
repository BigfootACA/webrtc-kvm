/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"video.h"

VideoProcess::VideoProcess(
	WebRTCScreen*screen,
	std::shared_ptr<rtc::Track>track
):screen(screen),track(track){
	log_info("video process started");
	track->onFrame([this](auto b,auto f){onFrame(b,f);});
	track->onOpen([this](){onOpen();});
	track->onClosed([this](){onClosed();});
	track->onError([this](auto s){onError(s);});
}

void VideoProcess::setSize(int w,int h){
	log_info("video display resize to {}x{}",w,h);
	auto alg=sizeof(void*);
	scaled.w=round_down(w,alg);
	scaled.h=round_down(h,alg);
}

void WebRTCScreen::onResizeVideo(QResizeEvent*event){
	if(!video_process)return;
	auto size=central->size();
	video_process->setSize(size.width(),size.height());
}

void VideoProcess::commitImage(QImage&image){
	screen->SubmitJob([this,image](){
		screen->video->resize(image.size());
		screen->video->setPixmap(QPixmap::fromImage(image));
	},Qt::BlockingQueuedConnection);
}

void VideoProcess::commitFrame(AVFrame*frame){
	auto fmt=AVPixelFormatToQImageFormat((AVPixelFormat)frame->format);
	if(fmt==QImage::Format::Format_Invalid)
		throw InvalidArgument("unsupported format");
	QImage image(frame->data[0],frame->width,frame->height,fmt);
	commitImage(image);
}
