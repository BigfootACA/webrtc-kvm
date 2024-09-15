/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef FFMPEG_SHARED_H
#define FFMPEG_SHARED_H
#include"../stream.h"
#include"string/strings.h"
#include"lib/exception.h"
#include"lib/readable.h"
#include"lib/log.h"
extern "C"{
#include<libavutil/imgutils.h>
}

extern uint32_t AVPixelFormatToFourcc(AVPixelFormat f);
extern AVPixelFormat FourccToAVPixelFormat(uint32_t f);
#endif
