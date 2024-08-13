/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"strings.h"
#include<string>
#include<linux/videodev2.h>

bool StringToH264Profile(const std::string&val,v4l2_mpeg_video_h264_profile&profile){
	if(val=="baseline")profile=V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE;
	else if(val=="constrained-baseline")profile=V4L2_MPEG_VIDEO_H264_PROFILE_CONSTRAINED_BASELINE;
	else if(val=="main")profile=V4L2_MPEG_VIDEO_H264_PROFILE_MAIN;
	else if(val=="extended")profile=V4L2_MPEG_VIDEO_H264_PROFILE_EXTENDED;
	else if(val=="high")profile=V4L2_MPEG_VIDEO_H264_PROFILE_HIGH;
	else if(val=="high-10")profile=V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_10;
	else if(val=="high-422")profile=V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_422;
	else if(val=="high-444-predictive")profile=V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_444_PREDICTIVE;
	else if(val=="high-10-intra")profile=V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_10_INTRA;
	else if(val=="high-422-intra")profile=V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_422_INTRA;
	else if(val=="high-444-intra")profile=V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_444_INTRA;
	else if(val=="cavlc-444-intra")profile=V4L2_MPEG_VIDEO_H264_PROFILE_CAVLC_444_INTRA;
	else if(val=="scalable-baseline")profile=V4L2_MPEG_VIDEO_H264_PROFILE_SCALABLE_BASELINE;
	else if(val=="scalable-high")profile=V4L2_MPEG_VIDEO_H264_PROFILE_SCALABLE_HIGH;
	else if(val=="scalable-high-intra")profile=V4L2_MPEG_VIDEO_H264_PROFILE_SCALABLE_HIGH_INTRA;
	else if(val=="stereo-high")profile=V4L2_MPEG_VIDEO_H264_PROFILE_STEREO_HIGH;
	else if(val=="multiview-high")profile=V4L2_MPEG_VIDEO_H264_PROFILE_MULTIVIEW_HIGH;
	else if(val=="constrained-high")profile=V4L2_MPEG_VIDEO_H264_PROFILE_CONSTRAINED_HIGH;
	else return false;
	return true;
}
