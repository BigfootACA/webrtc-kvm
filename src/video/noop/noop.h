/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef NOOP_H
#define NOOP_H
#include"../stream.h"

class NoopSink:public Stream{
	public:
		~NoopSink()override=default;
		explicit NoopSink(webrtc_kvm*ctx);
		[[nodiscard]] inline std::string GetDriverName()const final{return "noop-out";}
		[[nodiscard]] inline StreamType GetType()const override{return STREAM_SINK;}
		[[nodiscard]] inline uint32_t GetWidth()const override{return GetInputStream()->GetWidth();}
		[[nodiscard]] inline uint32_t GetHeight()const override{return GetInputStream()->GetHeight();}
		[[nodiscard]] inline uint32_t GetFrameRate()const override{return GetInputStream()->GetFrameRate();}
	protected:
		void OnInitialize()final;
		void OnDeinitialize()final;
		void OnStartStream()final;
		void OnStopStream()final;
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		void OnProcessInput(StreamBuffer*buffer)final;
};

class NoopPipe:public Stream{
	public:
		~NoopPipe()override=default;
		explicit NoopPipe(webrtc_kvm*ctx);
		[[nodiscard]] inline std::string GetDriverName()const final{return "noop-pipe";}
		[[nodiscard]] inline StreamType GetType()const override{return STREAM_PIPE;}
		[[nodiscard]] inline uint32_t GetWidth()const override{return GetInputStream()->GetWidth();}
		[[nodiscard]] inline uint32_t GetHeight()const override{return GetInputStream()->GetHeight();}
		[[nodiscard]] inline uint32_t GetFrameRate()const override{return GetInputStream()->GetFrameRate();}
	protected:
		void OnInitialize()final;
		void OnDeinitialize()final;
		void OnStartStream()final;
		void OnStopStream()final;
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		void OnProcessInput(StreamBuffer*buffer)final;
		void OnBindLink(std::shared_ptr<StreamLink>link,StreamLinkDirection dir)final;
};

#endif
