/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef V4L2_OUT_H
#define V4L2_OUT_H
#include"../dev/v4l2_dev.h"

class V4L2Output:public V4L2Device{
	friend class V4L2MemoryToMemory;
	public:
		~V4L2Output()override=default;
		explicit V4L2Output(webrtc_kvm*ctx);
		[[nodiscard]] inline std::string GetDriverName()const final{return "v4l2-out";}
		[[nodiscard]] inline StreamType GetType()const final{return STREAM_SINK;}
	protected:
		void PrepareInput(StreamBuffer*input,V4L2StreamBuffer*buffer);
		V4L2StreamBuffer*FindAvailableBuffer();
		void QueueInput(StreamBuffer*input,V4L2StreamBuffer*buffer);
		void OnProcessInput(StreamBuffer*buffer)final;
		void OnInitialize()final;
		void OnDeinitialize()final;
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		v4l2_buf_type DetectType(uint32_t cap)final;
};

class V4L2OutputFactory:public StreamFactory{
	public:
		~V4L2OutputFactory()override=default;
		inline V4L2OutputFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()const final{return "v4l2-out";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

#endif
