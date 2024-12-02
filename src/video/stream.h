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

enum StreamLinkDirection{
	LINK_DIR_UNKNOWN,
	LINK_DIR_IN,
	LINK_DIR_OUT,
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
	friend class Stream;
	public:
		StreamLink()=default;
		void Initialize(StreamList*stream,YAML::Node&cfg);
		void Initialize(Stream*source,Stream*sink,BufferType type=BUFFER_NULL);
		static void CreateLinksFromConfig(StreamList*list,YAML::Node&cfg);
		[[nodiscard]] inline uint32_t GetFourcc()const{return fourcc;}
		[[nodiscard]] inline BufferType GetType()const{return type;}
		[[nodiscard]] inline Stream*GetSource()const{return source;}
		[[nodiscard]] inline Stream*GetSink()const{return sink;}
		[[nodiscard]] virtual std::string ToString()const;
	protected:
		BufferType type=BUFFER_NULL;
		uint32_t fourcc=0;
		uint32_t planes_cnt=0;
		struct{
			size_t size;
		}planes[8]={};
		Stream*source=nullptr;
		Stream*sink=nullptr;
		void BindSource(Stream*stream);
		void BindSink(Stream*stream);
};

class Stream{
	friend class StreamLink;
	friend class StreamFactory;
	public:
		virtual ~Stream()=default;
		[[nodiscard]] virtual std::string GetDriverName()const=0;
		[[nodiscard]] inline UUID GetUUID()const{return uuid;}
		[[nodiscard]] inline std::string GetID()const{return id;}
		[[nodiscard]] inline std::string GetName()const{return name;}
		[[nodiscard]] inline std::shared_ptr<StreamLink>GetInput()const{return input;}
		[[nodiscard]] std::shared_ptr<StreamLink>GetOutput(size_t id=0)const;
		[[nodiscard]] inline StreamStatus GetStatus()const{return status;}
		[[nodiscard]] virtual StreamType GetType()const=0;
		[[nodiscard]] Stream*GetInputStream()const;
		[[nodiscard]] Stream*GetOutputStream(size_t id=0)const;
		void StartStream(bool all=true);
		void StopStream(bool all=true,bool deinit=true);
		[[nodiscard]] virtual uint32_t GetWidth()const=0;
		[[nodiscard]] virtual uint32_t GetHeight()const=0;
		[[nodiscard]] virtual uint32_t GetFrameRate()const=0;
	protected:
		virtual void OpenDevice();
		void LoadConfig(YAML::Node&cfg);
		void SendToNext(StreamBuffer*buf);
		void ProcessInput(StreamBuffer*buffer);
		[[nodiscard]] size_t&OutputPlaneSize(size_t id,uint32_t plane);
		[[nodiscard]] size_t&InputPlaneSize(uint32_t plane);
		[[nodiscard]] uint32_t&CurrentFourcc();
		void BindLink(std::shared_ptr<StreamLink>link,StreamLinkDirection dir);
		virtual void OnStartStream()=0;
		virtual void OnStopStream()=0;
		virtual void OnInitialize()=0;
		virtual void OnDeinitialize()=0;
		[[nodiscard]] inline virtual int GetFD()const{return device_fd;}
		inline virtual void OnLoadDeviceConfig(YAML::Node&cfg){}
		inline virtual void OnProcessInput(StreamBuffer*buffer){}
		inline virtual void OnBindLink(std::shared_ptr<StreamLink>link,StreamLinkDirection dir){}
		inline virtual void OnProcessNeedQueue(){}
		inline virtual void OnProcessOutput(){}
		virtual void ProcessEvent(EventHandlerContext*ev);
		virtual void EnablePoll(int events=EPOLLIN);
		virtual void DirectEnablePoll(int events=EPOLLIN);
		virtual void DisablePoll();
		virtual void DirectDisablePoll();
		void TimestampFromFrames(timeval&tv,uint64_t frame)const;
		int device_fd=-1;
		UUID poll_id;
		std::string path;
		uint32_t error=0;
		uint32_t fourcc_in=0;
		uint32_t fourcc_out=0;
		bool open_device=true;
		webrtc_kvm*ctx;
		StreamPipeRole role=ROLE_NONE;
		StreamStatus status=STREAM_UNINITIALIZE;
		std::shared_ptr<StreamLink>input;
		std::vector<std::shared_ptr<StreamLink>>outputs;
		std::string name,id;
		UUID uuid;
};

class StreamFactory{
	public:
		inline virtual ~StreamFactory(){UnregisterSelf();}
		[[nodiscard]] virtual std::string GetDriverName()const=0;
		[[nodiscard]] virtual Stream*Create(webrtc_kvm*ctx)=0;
		[[nodiscard]] virtual Stream*CreateFromConfig(webrtc_kvm*ctx,YAML::Node&cfg);
		[[nodiscard]] static StreamFactory*GetFactoryByDriverName(const std::string&driver,bool plugin=true);
		[[nodiscard]] static Stream*CreateStreamByDriverName(webrtc_kvm*ctx,const std::string&driver);
		[[nodiscard]] static Stream*CreateStreamFromConfig(webrtc_kvm*ctx,YAML::Node&cfg);
		static void CreateStreamsFromConfig(webrtc_kvm*ctx,StreamList*list,YAML::Node&cfg);
		static StreamFactory*LoadPlugin(const std::string&name);
	protected:
		void RegisterSelf();
		void UnregisterSelf();
	private:
		std::string driver_name;
		static struct StreamFactoryInfo*info;
};

class StreamList{
	public:
		virtual ~StreamList()=default;
		void Add(Stream*stream);
		void Remove(Stream*stream);
		[[nodiscard]] Stream*FindByUUID(const UUID&uuid)const;
		[[nodiscard]] Stream*FindByID(const std::string&id)const;
	private:
		std::map<UUID,Stream*>streams_by_uuid;
		std::map<std::string,Stream*>streams_by_id;
};

extern bool StringToPipeRole(const std::string&val,StreamPipeRole&role);

#define DECL_FACTORY_BUILTIN(cls,name) cdecl_attr_used cls video_##name##_factory;
#define DECL_FACTORY(cls,name) \
	DECL_FACTORY_BUILTIN(cls,name) \
	extern "C" StreamFactory*webrtc_plugin_video_get_##name(){\
		return &video_##name##_factory;\
	}
#endif
