/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef WEBRTC_CLIENT_SCREEN_VIDEO_H
#define WEBRTC_CLIENT_SCREEN_VIDEO_H
#include"../screen.h"
#include"lib/log.h"
#include"lib/exception.h"
extern "C"{
#include<libavcodec/avcodec.h>
#include<libavutil/imgutils.h>
#include<libswscale/swscale.h>
}

struct VideoFormat{
	int w,h;
	AVPixelFormat f;
	inline bool operator==(const VideoFormat&fmt){return fmt.w==w&&fmt.h==h&&fmt.f==f;}
	inline bool operator!=(const VideoFormat&fmt){return fmt.w!=w||fmt.h!=h||fmt.f!=f;}
};

class VideoProcess{
	public:
		explicit VideoProcess(WebRTCScreen*screen,std::shared_ptr<rtc::Track>track);
		~VideoProcess();
		void setSize(int w,int h);
	protected:
		void onFrame(rtc::binary b,rtc::FrameInfo f);
		void onOpen();
		void onClosed();
		void onError(std::string err);
	private:
		void commitImage(QImage&image);
		void commitFrame(AVFrame*frame);
		void destroyScaler();
		void destroyAVCodec();
		void initScaler(const VideoFormat&src,const VideoFormat&dst);
		void initScaledFrame(const VideoFormat&dst);
		bool receiveFrame();
		WebRTCScreen*screen;
		std::shared_ptr<rtc::Track>track=nullptr;
		std::string format;
		const AVCodec*codec=nullptr;
		AVCodecContext*ctx=nullptr;
		AVPacket packet{};
		AVFrame*decoded_frame=nullptr;
		AVFrame*scaled_frame=nullptr;
		uint8_t*scaled_buffer=nullptr;
		size_t scaled_buffer_size=0;
		SwsContext*sws=nullptr;
		std::mutex lock;
		VideoFormat scaled{.f=AV_PIX_FMT_RGB24};
		struct{
			VideoFormat src,dst;
		}last{};
};

extern QImage::Format AVPixelFormatToQImageFormat(AVPixelFormat f);
#endif
