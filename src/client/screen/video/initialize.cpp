/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"video.h"
#include<cctype>

static void avcodec_log_callback(void *avcl,int level,const char*fmt,va_list vl){
	log::log_level l=log::LOG_INFO;
	if(level>av_log_get_level())return;
	switch(level){
		case AV_LOG_PANIC:l=log::LOG_FATAL;break;
		case AV_LOG_FATAL:l=log::LOG_FATAL;break;
		case AV_LOG_ERROR:l=log::LOG_ERROR;break;
		case AV_LOG_WARNING:l=log::LOG_WARNING;break;
		case AV_LOG_INFO:l=log::LOG_INFO;break;
		case AV_LOG_VERBOSE:l=log::LOG_VERBOSE;break;
		case AV_LOG_DEBUG:l=log::LOG_DEBUG;break;
		case AV_LOG_TRACE:l=log::LOG_TRACE;break;
		default:;return;
	}
	char*msg=nullptr;
	auto r=vasprintf(&msg,fmt,vl);
	if(!msg)return;
	size_t len=strnlen(msg,r);
	while(len>0&&isspace(msg[len-1]))len--;
	if(r>0)log::print(l,std::string(msg,len));
	free(msg);
}

void VideoProcess::onOpen(){
	log_info("video track opened");
	auto desc=track->description();
	auto types=desc.payloadTypes();
	if(types.empty()){
		log_warn("video no payload type");
		return;
	}
	log_info("video payload type: {}",types[0]);
	auto map=desc.rtpMap(types[0]);
	if(!map){
		log_warn("video no rtp map");
		return;
	}
	format=map->format;
	log_info("video format: {}",format);
	AVCodecID codec_id=AV_CODEC_ID_NONE;
	std::shared_ptr<rtc::MediaHandler>depacketizer=nullptr;
	if(format=="H264"){
		depacketizer=std::make_shared<rtc::H264RtpDepacketizer>();
		codec_id=AV_CODEC_ID_H264;
	}else if(format=="H265"){
		codec_id=AV_CODEC_ID_HEVC;
	}else if(format=="AV1"){
		codec_id=AV_CODEC_ID_AV1;
	}else if(format=="VP8"){
		codec_id=AV_CODEC_ID_VP8;
	}else if(format=="VP9"){
		codec_id=AV_CODEC_ID_VP9;
	}
	if(depacketizer){
		auto session=std::make_shared<rtc::RtcpReceivingSession>();
		depacketizer->addToChain(session);
		track->setMediaHandler(depacketizer);
	}else log_warn("video no depacketizer");
	try{
		int ret;
		if(codec_id==AV_CODEC_ID_NONE)
			throw InvalidArgument("video no avcodec id");
		av_log_set_level(AV_LOG_INFO);
		av_log_set_callback(avcodec_log_callback);
		if(!(codec=avcodec_find_decoder(codec_id)))
			throw RuntimeError("avcodec decoder for {} not found",format);
		if(!(ctx=avcodec_alloc_context3(codec)))
			throw RuntimeError("avcodec context allocate failed");
		if((ret=avcodec_open2(ctx,codec,nullptr))<0)
			throw ErrnoExceptionWith(AVUNERROR(ret),"avcodec open failed");
		if(!(decoded_frame=av_frame_alloc()))
			throw RuntimeError("avcodec context decoded frame failed");
		if(!(scaled_frame=av_frame_alloc()))
			throw RuntimeError("avcodec context scaled frame failed");
		log_info("avcodec context initialized");
	}catch(std::exception&exc){
		log_warn("video decoder initialize failed: {}",exc.what());
	}
}
