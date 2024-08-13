/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"

void Stream::TimestampFromFrames(timeval&tv,uint64_t frame){
	uint64_t timestamp=frame*(1000000000ULL/GetFrameRate());
	tv.tv_sec=timestamp/1000000000ULL;
	tv.tv_usec=(timestamp%1000000000ULL)/1000UL;
}
