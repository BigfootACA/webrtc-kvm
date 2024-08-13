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

bool StringToH264Level(const std::string&val,v4l2_mpeg_video_h264_level&level){
	if(val=="1.0")level=V4L2_MPEG_VIDEO_H264_LEVEL_1_0;
	else if(val=="1B")level=V4L2_MPEG_VIDEO_H264_LEVEL_1B;
	else if(val=="1.1")level=V4L2_MPEG_VIDEO_H264_LEVEL_1_1;
	else if(val=="1.2")level=V4L2_MPEG_VIDEO_H264_LEVEL_1_2;
	else if(val=="1.3")level=V4L2_MPEG_VIDEO_H264_LEVEL_1_3;
	else if(val=="2.0")level=V4L2_MPEG_VIDEO_H264_LEVEL_2_0;
	else if(val=="2.1")level=V4L2_MPEG_VIDEO_H264_LEVEL_2_1;
	else if(val=="2.2")level=V4L2_MPEG_VIDEO_H264_LEVEL_2_2;
	else if(val=="3.0")level=V4L2_MPEG_VIDEO_H264_LEVEL_3_0;
	else if(val=="3.1")level=V4L2_MPEG_VIDEO_H264_LEVEL_3_1;
	else if(val=="3.2")level=V4L2_MPEG_VIDEO_H264_LEVEL_3_2;
	else if(val=="4.0")level=V4L2_MPEG_VIDEO_H264_LEVEL_4_0;
	else if(val=="4.1")level=V4L2_MPEG_VIDEO_H264_LEVEL_4_1;
	else if(val=="4.2")level=V4L2_MPEG_VIDEO_H264_LEVEL_4_2;
	else if(val=="5.0")level=V4L2_MPEG_VIDEO_H264_LEVEL_5_0;
	else if(val=="5.1")level=V4L2_MPEG_VIDEO_H264_LEVEL_5_1;
	else if(val=="5.2")level=V4L2_MPEG_VIDEO_H264_LEVEL_5_2;
	else if(val=="6.0")level=V4L2_MPEG_VIDEO_H264_LEVEL_6_0;
	else if(val=="6.1")level=V4L2_MPEG_VIDEO_H264_LEVEL_6_1;
	else if(val=="6.2")level=V4L2_MPEG_VIDEO_H264_LEVEL_6_2;
	else return false;
	return true;
}

