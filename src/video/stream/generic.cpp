/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/exception.h"

void Stream::TimestampFromFrames(timeval&tv,uint64_t frame)const{
	uint64_t timestamp=frame*(1000000000ULL/GetFrameRate());
	tv.tv_sec=timestamp/1000000000ULL;
	tv.tv_usec=(timestamp%1000000000ULL)/1000UL;
}

uint32_t&Stream::CurrentFourcc(){
	switch(GetType()){
		case STREAM_SINK:return fourcc_in;
		case STREAM_SOURCE:return fourcc_out;
		case STREAM_PIPE:return fourcc_out;
		default:throw InvalidArgument("bad stream type");
	}
}
