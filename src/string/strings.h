/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef STRINGS_H
#define STRINGS_H
#include<string>
#include<cstdint>
#include<cstring>
#include<linux/videodev2.h>
#include<rtc/rtc.hpp>
extern bool StringToFourcc(const std::string&val,uint32_t&fourcc);
extern std::string FourccToString(uint32_t fourcc);
extern bool StringToH264Level(const std::string&val,v4l2_mpeg_video_h264_level&level);
extern bool StringToH264Profile(const std::string&val,v4l2_mpeg_video_h264_profile&profile);
extern bool StringToFourcc(const std::string&val,uint32_t&fourcc);
extern bool StringToV4L2BufType(const std::string&val,v4l2_buf_type&type);
extern bool StringToV4L2Memory(const std::string&val,v4l2_memory&mem);
extern std::string RTCPeerConnectionStateToString(rtc::PeerConnection::State state);
extern std::string RTCPeerConnectionGatheringStateToString(rtc::PeerConnection::GatheringState state);
#endif
