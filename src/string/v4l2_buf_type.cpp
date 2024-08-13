/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"strings.h"
#include<string>

bool StringToV4L2BufType(const std::string&val,v4l2_buf_type&type){
	if(val=="video-capture")type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	else if(val=="video-output")type=V4L2_BUF_TYPE_VIDEO_OUTPUT;
	else if(val=="video-overlay")type=V4L2_BUF_TYPE_VIDEO_OVERLAY;
	else if(val=="vbi-capture")type=V4L2_BUF_TYPE_VBI_CAPTURE;
	else if(val=="vbi-output")type=V4L2_BUF_TYPE_VBI_OUTPUT;
	else if(val=="sliced-vbi-capture")type=V4L2_BUF_TYPE_SLICED_VBI_CAPTURE;
	else if(val=="sliced-vbi-output")type=V4L2_BUF_TYPE_SLICED_VBI_OUTPUT;
	else if(val=="video-output-overlay")type=V4L2_BUF_TYPE_VIDEO_OUTPUT_OVERLAY;
	else if(val=="video-capture-mplane")type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	else if(val=="video-output-mplane")type=V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	else if(val=="sdr-capture")type=V4L2_BUF_TYPE_SDR_CAPTURE;
	else if(val=="sdr-output")type=V4L2_BUF_TYPE_SDR_OUTPUT;
	else if(val=="meta-capture")type=V4L2_BUF_TYPE_META_CAPTURE;
	else if(val=="meta-output")type=V4L2_BUF_TYPE_META_OUTPUT;
	else if(val=="private")type=V4L2_BUF_TYPE_PRIVATE;
	else return false;
	return true;
}
