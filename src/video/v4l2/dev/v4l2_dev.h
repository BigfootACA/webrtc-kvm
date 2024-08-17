/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef V4L2_DEV_H
#define V4L2_DEV_H
#include"lib/log.h"
#include"../v4l2.h"
#include"../../stream.h"
#include<sys/mman.h>
#include<linux/videodev2.h>
#include<linux/dma-buf.h>

enum BufferStatus{
	STATUS_IDLE     = 1,
	STATUS_PREPARED = 2,
	STATUS_QUEUED   = 3,
	STATUS_DEQUEUED = 4,
};

class V4L2StreamBuffer{
	public:
		uint32_t idx=0;
		v4l2_buffer buffer={};
		v4l2_plane planes[VIDEO_MAX_PLANES]={};
		void*mmap_data[VIDEO_MAX_PLANES]={nullptr};
		int dmabuf_fds[VIDEO_MAX_PLANES]={-1};
		BufferStatus status=STATUS_IDLE;
};

class V4L2Device:public Stream{
	public:
		~V4L2Device()override=default;
		[[nodiscard]] inline uint32_t GetWidth()const override{return width;}
		[[nodiscard]] inline uint32_t GetHeight()const override{return height;}
		[[nodiscard]] inline uint32_t GetFrameRate()const override{return fps;}
	protected:
		void OnStartStream()override{v4l2_stream_on(device_fd,type);}
		void OnStopStream()override{v4l2_stream_off(device_fd,type);}
		void OnDeinitialize()override;
		void InitializeDevice();
		void InitializeBuffers();
		void InitializeBuffer(V4L2StreamBuffer*buffer);
		void InitializeBufferMMAP(V4L2StreamBuffer*buffer);
		void InitializeBufferDMABUF(V4L2StreamBuffer*buffer);
		void InitializeBufferUPTR(V4L2StreamBuffer*buffer);
		void DequeueBuffer(V4L2StreamBuffer**buffer=nullptr);
		void QueueBuffer(V4L2StreamBuffer*buffer);
		void SetupFormat();
		void SetupBuffer();
		void FillBuffer(V4L2StreamBuffer*buffer);
		void LoadGenericConfig(YAML::Node&cfg);
		void LoadMatchConfig(YAML::Node&cfg);
		void LoadControlsConfig(YAML::Node&obj);
		void FindMatchDevice();
		void OnBindLink(std::shared_ptr<StreamLink>link,StreamLinkDirection dir)override;
		virtual void SetupDevice(){}
		virtual v4l2_buf_type DetectType(uint32_t cap)=0;
		bool use_dmabuf=false;
		v4l2_buf_type type=(v4l2_buf_type)0;
		v4l2_memory memory=(v4l2_memory)0;
		uint64_t frames=0;
		uint32_t fps=0;
		uint32_t width=0,height=0;
		uint32_t plane_count=0;
		uint32_t buffer_cap=0;
		uint32_t device_cap=0;
		uint32_t size[VIDEO_MAX_PLANES]={};
		v4l2_format format{};
		uint32_t buffer_cnt=3;
		std::vector<V4L2StreamBuffer*>buffers;
		std::unordered_map<uint32_t,int32_t>controls;
		struct{
			std::string driver;
			std::string card;
		}match={};
};
#endif
