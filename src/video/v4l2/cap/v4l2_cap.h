/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef V4L2_CAP_H
#define V4L2_CAP_H
#include"../dev/v4l2_dev.h"
class V4L2Capture:public V4L2Device{
	public:
		virtual ~V4L2Capture()=default;
		inline V4L2Capture(webrtc_kvm*ctx){this->ctx=ctx;}
		[[nodiscard]] inline std::string GetDriverName()final{return "v4l2-cap";}
		inline StreamType GetType()override{return STREAM_SOURCE;}
		void OnProcessOutput()final;
		void OnProcessNeedQueue()final;
		void ProcessNextInput(V4L2StreamBuffer*in);
		void OnInitialize()final;
		void OnDeinitialize()final;
		virtual void OnStartStream()final;
		virtual void OnStopStream()final;
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		void OnBindOutput(std::shared_ptr<StreamLink>link)final;
		void OnProcessInput(StreamBuffer*buffer)final{}
		v4l2_buf_type DetectType(uint32_t cap)final;
};
class V4L2CaptureFactory:public StreamFactory{
	public:
		~V4L2CaptureFactory()override=default;
		inline V4L2CaptureFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()final{return "v4l2-cap";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

#endif
