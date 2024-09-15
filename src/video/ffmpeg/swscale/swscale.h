/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef FFMPEG_SWSCALE_H
#define FFMPEG_SWSCALE_H
#include"../ffmpeg.h"
extern "C"{
#include<libswscale/swscale.h>
}

class SwscalePipe:public Stream{
	public:
		~SwscalePipe()override=default;
		explicit SwscalePipe(webrtc_kvm*ctx);
		[[nodiscard]] inline std::string GetDriverName()const final{return "swscale";}
		[[nodiscard]] inline StreamType GetType()const final{return STREAM_PIPE;}
		[[nodiscard]] inline uint32_t GetWidth()const final{return width;}
		[[nodiscard]] inline uint32_t GetHeight()const final{return height;}
		[[nodiscard]] inline uint32_t GetFrameRate()const final{return GetInputStream()->GetFrameRate();}
	protected:
		void OnInitialize()final;
		void OnDeinitialize()final;
		void OnStartStream()final;
		void OnStopStream()final;
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		void OnProcessInput(StreamBuffer*buffer)final;
		uint32_t width=0,height=0;
		uint32_t in_width=0,in_height=0;
		AVPixelFormat fmt_in,fmt_out;
		SwsContext*sws=nullptr;
		size_t scaled_buffer_size[8]{};
		uint8_t*scaled_buffer[8]{};
		int in_planes=0,out_planes=0;
};

#endif
