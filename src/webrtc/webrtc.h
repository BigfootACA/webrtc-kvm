/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef WEBRTC_H
#define WEBRTC_H
#include<string>
#include<rtc/rtc.hpp>
#include<json/json.h>
#include"webrtc_kvm.h"
#include"lib/exception.h"
#include"lib/log.h"
#include"algo/uuid.h"
#include"algo/blob.h"
#include"video/stream.h"

class WebRTCEndpoint;
class WebRTCInstance;

struct TrackInfo{
	std::shared_ptr<rtc::Track>track;
	std::shared_ptr<rtc::RtcpSrReporter>sr;
	std::shared_ptr<rtc::RtcpNackResponder>nack;
	std::shared_ptr<rtc::RtpPacketizationConfig>config;
};

class WebRTCInstance{
	friend class WebRTCEndpoint;
	public:
		WebRTCInstance();
		~WebRTCInstance();
		[[nodiscard]] std::string GetSDP()const;
		void SetSDP(const std::string&sdp);
		static void NewInstance(WebRTCEndpoint*ep,std::pair<UUID,WebRTCInstance*>&ins);
	protected:
		void Init();
		void ProcessVideoStream(const Blob&data);
		void VideoTrackInit(
			TrackInfo&track,
			int payload,int ssrc,
			const char*mid,const char*msid
		);
		void EnableStream();
		void DisableStream();
		void OnStateChange(rtc::PeerConnection::State state);
		void OnGatheringStateChange(rtc::PeerConnection::GatheringState state);
		void OnInputMessage(rtc::message_variant data);
		void OnEventMessage(rtc::message_variant data);
		void OnDataChannel(std::shared_ptr<rtc::DataChannel>ch);
		void SendEvent(Json::Value&node);
		void SendEvent(const std::string&str);
		std::mutex lock;
		webrtc_kvm*kvm=nullptr;
		WebRTCEndpoint*ep=nullptr;
		TrackInfo video;
		std::shared_ptr<rtc::PeerConnection>peer;
		std::shared_ptr<rtc::DataChannel>input;
		std::shared_ptr<rtc::DataChannel>event;
		rtc::PeerConnection::State state;
		rtc::PeerConnection::GatheringState gathering_state;
		std::string local_sdp;
		stream_callback callback;
		UUID uuid;
};

class WebRTCEndpoint:public Stream{
	friend class WebRTCInstance;
	public:
		inline explicit WebRTCEndpoint(webrtc_kvm*kvm):kvm(kvm){}
		[[nodiscard]] inline std::string GetDriverName()const final{return "webrtc";}
		[[nodiscard]] inline StreamType GetType()const final{return STREAM_SINK;}
		[[nodiscard]] inline uint32_t GetWidth()const final{return GetInputStream()->GetWidth();}
		[[nodiscard]] inline uint32_t GetHeight()const final{return GetInputStream()->GetHeight();}
		[[nodiscard]] inline uint32_t GetFrameRate()const final{return GetInputStream()->GetFrameRate();}
		[[nodiscard]] WebRTCInstance*GetInstance(const UUID&uuid);
		void NewInstance(std::pair<UUID,WebRTCInstance*>&ins);
	protected:
		void ReportSize();
		void OnStartStream()final;
		void OnStopStream()final;
		void OnInitialize()final;
		void OnDeinitialize()final;
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		void OnProcessInput(StreamBuffer*buffer)final;
		void EnableStream(const UUID&uuid);
		void DisableStream(const UUID&uuid);
		void RemoveStream(const UUID&uuid);
		void SendEventAll(Json::Value&node);
		std::map<UUID,WebRTCInstance*>instances;
		std::vector<WebRTCInstance*>streaming;
		std::mutex lock;
		webrtc_kvm*kvm=nullptr;
		uint64_t frames=0;
};
#endif
