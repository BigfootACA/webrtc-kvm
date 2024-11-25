/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef RKMPP_H
#define RKMPP_H
#include"../stream.h"
#include<rockchip/vpu.h>
#include<rockchip/rk_mpi.h>
#include<rockchip/mpp_log.h>
#include<rockchip/mpp_frame.h>

#define MPP_ISERR(expr) (unlikely((ret=(expr))!=MPP_OK))

struct MppConfigItem{
	std::string key;
	enum MppConfigValueType{
		TYPE_NONE,
		TYPE_S32,
		TYPE_U32,
		TYPE_S64,
		TYPE_U64,
	}type=TYPE_NONE;
	union{
		int32_t s32;
		uint32_t u32;
		int64_t s64;
		uint64_t u64;
	}v{};
};

class MppConfigList{
	public:
		inline void Clear(){config.clear();}
		void AddS32(const std::string&key,int32_t v);
		void AddU32(const std::string&key,uint32_t v);
		void AddS64(const std::string&key,int64_t v);
		void AddU64(const std::string&key,uint64_t v);
		void FromYAML(YAML::Node&cfg);
		void FillToEncoder(MppEncCfg cfg);
	private:
		std::unordered_map<std::string,MppConfigItem>config;
};

struct MppBufferDesc{
	int fd;
	MppBufferInfo info;
	MppBuffer buff;
};

class RockchipMediaProcessPlatform:public Stream{
	public:
		~RockchipMediaProcessPlatform()override=default;
		explicit RockchipMediaProcessPlatform(webrtc_kvm*ctx);
		[[nodiscard]] inline std::string GetDriverName()const final{return "rkmpp";}
		[[nodiscard]] inline StreamType GetType()const override{return STREAM_PIPE;}
		[[nodiscard]] inline uint32_t GetWidth()const override{return width;}
		[[nodiscard]] inline uint32_t GetHeight()const override{return height;}
		[[nodiscard]] inline uint32_t GetFrameRate()const override{return fps;}
		static uint32_t MppCodingTypeToFourcc(MppCodingType type);
		static MppCodingType FourccToMppCodingType(uint32_t fourcc);
		static uint32_t MppFrameFormatToFourcc(MppFrameFormat fmt);
		static MppFrameFormat FourccToMppFrameFormat(uint32_t fourcc);
		static size_t CalcFrameSize(MppFrameFormat fmt,size_t hor_stride,size_t ver_stride);
		static size_t EncoderWidthDefaultStride(size_t width,MppFrameFormat fmt);
	protected:
		void EncoderPreInit();
		void EncoderPostInit();
		void DecoderPreInit();
		void DecoderPostInit();
		void OnInitialize()final;
		void OnDeinitialize()final;
		void OnStartStream()final;
		void OnStopStream()final;
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		void OnProcessInput(StreamBuffer*buffer)final;
		void OnProcessInputEncoder(MppBuffer buf,StreamBuffer*buffer);
		void OnProcessInputDecoder(MppBuffer buf,StreamBuffer*buffer);
		void WriteHeader();
		MppBuffer SetupFrameDMABUF(StreamBuffer*buffer);
		MppBuffer SetupFramePointer(StreamBuffer*buffer);
		void WritePacket(MppPacket packet);
		void WriteFrame(MppFrame frame);
		void Reset();
		MppEncCfg enc_cfg=nullptr;
		MppCtxType ctx_type=MPP_CTX_BUTT;
		MppCodingType video_type=MPP_VIDEO_CodingUnused;
		MppFrameFormat buff_fmt=MPP_FMT_BUTT;
		size_t hor_stride=0,ver_stride=0;
		uint32_t width=0,height=0;
		uint32_t fps=30;
		uint64_t frames=0;
		size_t frame_size=0;
		MppCtx mpp_ctx=nullptr;
		MppBuffer frm_buf=nullptr;
		MppBuffer pkt_buf=nullptr;
		MppBufferGroup buf_grp=nullptr;
		MppApi*mpi=nullptr;
		MppFrame enc_frm=nullptr;
		MppPacket dec_pkt=nullptr;
		MppConfigList config_tempory;
		MppConfigList config_persist;
		MppPacket sps_pkt=nullptr;
		std::map<int,std::shared_ptr<MppBufferDesc>>dmabufs;
};

#endif
