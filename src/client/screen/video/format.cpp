/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"video.h"

QImage::Format AVPixelFormatToQImageFormat(AVPixelFormat f){
	switch(f){
		case AV_PIX_FMT_RGB32:return QImage::Format::Format_RGB32;
		case AV_PIX_FMT_ARGB:return QImage::Format::Format_ARGB32;
		case AV_PIX_FMT_RGB565:return QImage::Format::Format_RGB16;
		case AV_PIX_FMT_RGB555:return QImage::Format::Format_RGB555;
		case AV_PIX_FMT_RGB24:return QImage::Format::Format_RGB888;
		case AV_PIX_FMT_RGB444:return QImage::Format::Format_RGB444;
		case AV_PIX_FMT_BGR32:return QImage::Format::Format_RGBA8888;
		case AV_PIX_FMT_X2RGB10:return QImage::Format::Format_BGR30;
		case AV_PIX_FMT_X2BGR10:return QImage::Format::Format_RGB30;
		case AV_PIX_FMT_BGR24:return QImage::Format::Format_BGR888;
		default:return QImage::Format::Format_Invalid;
	}
}
