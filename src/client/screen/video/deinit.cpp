/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"video.h"

void VideoProcess::destroyScaler(){
	if(sws){
		sws_frame_end(sws);
		sws=nullptr;
	}
	if(scaled_buffer){
		av_free(scaled_buffer);
		scaled_buffer=nullptr;
		if(scaled_frame){
			memset(scaled_frame->data,0,sizeof(scaled_frame->data));
			memset(scaled_frame->linesize,0,sizeof(scaled_frame->linesize));
		}
	}
}

void VideoProcess::destroyAVCodec(){
	if(ctx){
		avcodec_free_context(&ctx);
		ctx=nullptr;
	}
	if(decoded_frame){
		av_frame_free(&decoded_frame);
		decoded_frame=nullptr;
	}
	if(scaled_frame){
		av_frame_free(&scaled_frame);
		scaled_frame=nullptr;
	}
}

VideoProcess::~VideoProcess(){
	log_info("video process stopped");
	if(track->isOpen())track->close();
	destroyAVCodec();
	destroyScaler();
}
