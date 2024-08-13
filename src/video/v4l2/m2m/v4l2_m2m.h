/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef V4L2_M2M_H
#define V4L2_M2M_H
#include"../cap/v4l2_cap.h"
#include"../out/v4l2_out.h"

class H264Options{
	public:
		v4l2_mpeg_video_h264_profile profile=V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE;
		v4l2_mpeg_video_h264_level level=V4L2_MPEG_VIDEO_H264_LEVEL_5_0;
		uint32_t qp_i=24;
		uint32_t qp_p=26;
		uint32_t gop_size=3;
		bool gop_closure=false;
};

class V4L2MemoryToMemory:public V4L2Device{
	public:
		~V4L2MemoryToMemory()override;
		explicit V4L2MemoryToMemory(webrtc_kvm*ctx);
		[[nodiscard]] inline std::string GetDriverName()override{return "v4l2-m2m";}
		inline StreamType GetType()override{return STREAM_PIPE;}
		void OnProcessOutput()final;
		void OnInitialize()final;
		void OnInitializeEncoder();
		void OnInitializeDecoder();
		void OnDeinitialize()final;
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		void OnProcessInput(StreamBuffer*buffer)final;
		void OnBindInput(std::shared_ptr<StreamLink>link)final;
		void OnBindOutput(std::shared_ptr<StreamLink>link)final;
		void OnStartStream()final;
		void OnStopStream()final;
		void OnProcessNeedQueue()final;
		v4l2_buf_type DetectType(uint32_t cap)final;
		void LoadH264EncoderConfig(YAML::Node obj);
	protected:
		V4L2Capture*cap=nullptr;
		V4L2Output*out=nullptr;
		H264Options options_h264;
};

class V4L2MemoryToMemoryFactory:public StreamFactory{
	public:
		~V4L2MemoryToMemoryFactory()override=default;
		inline V4L2MemoryToMemoryFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()final{return "v4l2-m2m";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

#endif
