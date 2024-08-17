/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"webrtc.h"
#include"string/strings.h"

static void fourccToCodec(rtcCodec&codec,uint32_t fourcc){
	switch(fourcc){
		case V4L2_PIX_FMT_H264:
		case V4L2_PIX_FMT_H264_MVC:
		case V4L2_PIX_FMT_H264_NO_SC:
		case V4L2_PIX_FMT_H264_SLICE:
			codec=RTC_CODEC_H264;
		break;
		case V4L2_PIX_FMT_HEVC:
		case V4L2_PIX_FMT_HEVC_SLICE:
			codec=RTC_CODEC_H265;
		break;
		case V4L2_PIX_FMT_AV1_FRAME:
			codec=RTC_CODEC_AV1;
		break;
		case V4L2_PIX_FMT_VP8:
		case V4L2_PIX_FMT_VP8_FRAME:
			codec=RTC_CODEC_VP8;
		break;
		case V4L2_PIX_FMT_VP9:
		case V4L2_PIX_FMT_VP9_FRAME:
			codec=RTC_CODEC_VP9;
		break;
		default:throw InvalidArgument(
			"target fourcc {} unsupported",
			FourccToString(fourcc)
		);
	}
}

void WebRTCInstance::VideoTrackInit(
	TrackInfo&track,
	int payload,int ssrc,
	const char*mid,const char*msid
){
	rtcCodec codec;
	fourccToCodec(codec,ep->fourcc_in);
	log_info(
		"webrtc {} create video track {} with payload {} ssrc {} mid {} msid {}",
		uuid.ToString(),FourccToString(ep->fourcc_in),payload,ssrc,mid,msid
	);
	auto desc=rtc::Description::Video(mid);
	switch(codec){
        	case RTC_CODEC_H264:desc.addH264Codec(payload);break;
        	case RTC_CODEC_H265:desc.addH265Codec(payload);break;
		default:throw InvalidArgument("unsupported codec");
        }
	desc.addSSRC(ssrc,mid,msid,mid);
	track.track=peer->addTrack(desc);
	track.config=std::make_shared<rtc::RtpPacketizationConfig>(ssrc,mid,payload,1);
	std::shared_ptr<rtc::RtpPacketizer>rtp;
	switch(codec){
		case RTC_CODEC_H264:
			track.config->clockRate=rtc::H264RtpPacketizer::defaultClockRate;
			rtp=make_shared<rtc::H264RtpPacketizer>(rtc::NalUnit::Separator::StartSequence,track.config);
		break;
		case RTC_CODEC_H265:
			track.config->clockRate=rtc::H265RtpPacketizer::defaultClockRate;
			rtp=make_shared<rtc::H265RtpPacketizer>(rtc::NalUnit::Separator::StartSequence,track.config);
		break;
		default:throw InvalidArgument("unsupported codec");
	}
	track.sr=make_shared<rtc::RtcpSrReporter>(track.config);
	rtp->addToChain(track.sr);
	track.nack=std::make_shared<rtc::RtcpNackResponder>();
	rtp->addToChain(track.nack);
	track.track->setMediaHandler(rtp);
}
