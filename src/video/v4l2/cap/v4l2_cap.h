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
#include"algo/blob.h"

class V4L2Capture:public V4L2Device{
	friend class V4L2MemoryToMemory;
	public:
		~V4L2Capture()override=default;
		inline explicit V4L2Capture(webrtc_kvm*ctx){this->ctx=ctx;}
		[[nodiscard]] inline std::string GetDriverName()const final{return "v4l2-cap";}
		[[nodiscard]] inline StreamType GetType()const override{return STREAM_SOURCE;}
		void ProcessNextInput(V4L2StreamBuffer*in);
	protected:
		void OnProcessOutput()final;
		void OnProcessNeedQueue()final;
		void OnInitialize()final;
		void OnDeinitialize()final;
		void OnStartStream()final;
		void OnStopStream()final;
		void LoadEdidConfig(YAML::Node&cfg);
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		void OnBindLink(std::shared_ptr<StreamLink>link,StreamLinkDirection dir)final;
		void OnProcessInput(StreamBuffer*buffer)final{}
		void SetupDevice()final;
		void SetupEdid();
		v4l2_buf_type DetectType(uint32_t cap)final;
		std::shared_ptr<Blob>edid=nullptr;
};
class V4L2CaptureFactory:public StreamFactory{
	public:
		~V4L2CaptureFactory()override=default;
		inline V4L2CaptureFactory(){RegisterSelf();}
		[[nodiscard]] inline std::string GetDriverName()const final{return "v4l2-cap";}
		[[nodiscard]] Stream*Create(webrtc_kvm*ctx)final;
};

#endif
