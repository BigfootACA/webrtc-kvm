/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef STREAM_H
#define STREAM_H
#include<yaml-cpp/yaml.h>
#include"algo/uuid.h"
#include"abstract/loop.h"
#include"webrtc_kvm.h"

class Stream;
class StreamList;

enum BufferType{
	BUFFER_NULL = 0,
	BUFFER_POINTER = 1,
	BUFFER_DMABUF = 2,
};

enum StreamStatus{
	STREAM_UNINITIALIZE = 1,
	STREAM_STOPPED = 2,
	STREAM_RUNNING = 3,
};

enum StreamType{
	STREAM_SOURCE = 1,
	STREAM_PIPE = 2,
	STREAM_SINK = 3,
};

enum StreamPipeRole{
	ROLE_NONE,
	ROLE_ENCODER,
	ROLE_DECODER,
	ROLE_CONVERTER,
};

struct StreamBuffer{
	BufferType type;
	uint32_t planes_cnt;
	struct{
		union{
			int fd;
			void*ptr;
		};
		size_t used,size;
	}planes[8];
	timeval timestamp;
};

class StreamLink:public std::enable_shared_from_this<StreamLink>{
	public:
		StreamLink()=default;
		void Initialize(StreamList*stream,YAML::Node&cfg);
		void Initialize(Stream*source,Stream*sink,BufferType type=BUFFER_NULL);
		BufferType type=BUFFER_NULL;
		uint32_t fourcc=0;
		uint32_t planes_cnt=0;
		struct{
			size_t size;
		}planes[8]={};
		Stream*source=nullptr;
		Stream*sink=nullptr;
		static void CreateLinksFromConfig(StreamList*list,YAML::Node&cfg);
		virtual std::string ToString();
		void BindSource(Stream*stream);
		void BindSink(Stream*stream);
		inline Stream*GetSource(){return source;}
		inline Stream*GetSink(){return sink;}
};

class Stream{
	public:
		virtual ~Stream()=default;
		void LoadConfig(YAML::Node&cfg);
		void ProcessInput(StreamBuffer*buffer);
		virtual std::string GetDriverName()=0;
		virtual void OpenDevice();
		inline UUID GetUUID(){return uuid;}
		inline std::string GetID(){return id;}
		inline std::string GetName(){return name;}
		inline std::shared_ptr<StreamLink>GetInput(){return input;}
		inline std::shared_ptr<StreamLink>GetOutput(){return output;}
		inline Stream*GetInputStream(){return input?input->GetSource():nullptr;}
		inline Stream*GetOutputStream(){return output?output->GetSink():nullptr;}
		inline StreamStatus GetStatus(){return status;}
		virtual StreamType GetType()=0;
		void StartStream(bool all=true);
		void StopStream(bool all=true,bool deinit=true);
		virtual uint32_t GetWidth()=0;
		virtual uint32_t GetHeight()=0;
		virtual uint32_t GetFrameRate()=0;
		virtual void OnStartStream()=0;
		virtual void OnStopStream()=0;
		virtual void OnInitialize()=0;
		virtual void OnDeinitialize()=0;
		inline virtual int GetFD(){return device_fd;}
		inline virtual void OnLoadDeviceConfig(YAML::Node&cfg){}
		inline virtual void OnProcessInput(StreamBuffer*buffer){}
		inline virtual void OnBindInput(std::shared_ptr<StreamLink>link){}
		inline virtual void OnBindOutput(std::shared_ptr<StreamLink>link){}
		inline virtual void OnProcessNeedQueue(){}
		inline virtual void OnProcessOutput(){}
		virtual void ProcessEvent(EventHandlerContext*ev);
		virtual void EnablePoll(int events=EPOLLIN);
		virtual void DirectEnablePoll(int events=EPOLLIN);
		virtual void DisablePoll();
		virtual void DirectDisablePoll();
		void TimestampFromFrames(timeval&tv,uint64_t frame);
		int device_fd=-1;
		UUID poll_id;
		std::string path;
		uint32_t error=0;
		bool open_device=true;
		webrtc_kvm*ctx;
		StreamPipeRole role=ROLE_NONE;
		StreamStatus status=STREAM_UNINITIALIZE;
		std::shared_ptr<StreamLink>input,output;
		std::string name,id;
		UUID uuid;
};

class StreamFactory{
	public:
		void RegisterSelf();
		void UnregisterSelf();
		inline virtual ~StreamFactory(){UnregisterSelf();}
		[[nodiscard]] virtual std::string GetDriverName()=0;
		[[nodiscard]] virtual Stream*Create(webrtc_kvm*ctx)=0;
		[[nodiscard]] virtual Stream*CreateFromConfig(webrtc_kvm*ctx,YAML::Node&cfg);
		static StreamFactory*GetFactoryByDriverName(const std::string&driver);
		static Stream*CreateStreamByDriverName(webrtc_kvm*ctx,const std::string&driver);
		static Stream*CreateStreamFromConfig(webrtc_kvm*ctx,YAML::Node&cfg);
		static void CreateStreamsFromConfig(webrtc_kvm*ctx,StreamList*list,YAML::Node&cfg);
		std::string driver_name;
		static struct StreamFactoryInfo*info;
};

class StreamList{
	public:
		virtual ~StreamList()=default;
		void Add(Stream*stream);
		void Remove(Stream*stream);
		Stream*FindByUUID(const UUID&uuid);
		Stream*FindByID(const std::string&id);
		std::map<UUID,Stream*>streams_by_uuid;
		std::map<std::string,Stream*>streams_by_id;
};

extern bool StringToPipeRole(const std::string&val,StreamPipeRole&role);
#endif
