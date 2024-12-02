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

class V4L2MemoryToMemory:public V4L2Device{
	public:
		~V4L2MemoryToMemory()override;
		explicit V4L2MemoryToMemory(webrtc_kvm*ctx);
		[[nodiscard]] inline std::string GetDriverName()const final{return "v4l2-m2m";}
		[[nodiscard]] inline StreamType GetType()const final{return STREAM_PIPE;}
	protected:
		void OnProcessOutput()final;
		void OnInitialize()final;
		void OnInitializeEncoder();
		void OnInitializeDecoder();
		void OnDeinitialize()final;
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		void OnProcessInput(StreamBuffer*buffer)final;
		void OnBindLink(std::shared_ptr<StreamLink>link,StreamLinkDirection dir)final;
		void OnStartStream()final;
		void OnStopStream()final;
		void OnProcessNeedQueue()final;
		v4l2_buf_type DetectType(uint32_t cap)final;
		V4L2Capture*cap=nullptr;
		V4L2Output*out=nullptr;
};

class V4L2MemoryToMemoryFactory:public StreamFactory{
	public:
		~V4L2MemoryToMemoryFactory()override=default;
		inline V4L2MemoryToMemoryFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()const final{return "v4l2-m2m";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

#endif
