/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef RKRGA_H
#define RKRGA_H
#include"stream.h"
#include<rga/rga.h>
#include<rga/im2d.h>
#include<rga/RgaUtils.h>
#include<sys/mman.h>

struct RGABuffers;

enum RgaAction{
	RGA_NONE,
	RGA_COPY,
	RGA_RESIZE,
	RGA_CROP,
	RGA_TRANSLATE,
	RGA_CONVERT,
	RGA_ROTATE,
	RGA_FILP,
	RGA_COLORKEY,
	RGA_ROP,
	RGA_FILL,
	RGA_RECTANGLE,
	RGA_BORDER,
	RGA_MOSAIC,
};

struct RGAImageDesc{
	int width=0;
	int height=0;
	RgaSURF_FORMAT format=RK_FORMAT_UNKNOWN;
	size_t size=0;
	bool auto_size=true;
	bool Empty();
	void FillMissing(const RGAImageDesc&desc);
	void ParseConfig(YAML::Node&cfg);
	void Update();
};

struct RGABuffer{
	size_t index=0;
	bool allocated=false;
	void*pointer=nullptr;
	int dmabuf_fd=-1;
	rga_buffer_handle_t handle{};
	rga_buffer_t buffer{};
	RGAImageDesc desc{};
	inline ~RGABuffer(){Destroy();}
	void Import(StreamBuffer*buffer);
	void Create(int dmabuf_dev_fd);
	void Destroy();
};

struct RGABuffers{
	RGAImageDesc desc{};
	size_t count=1,current=0;
	std::vector<std::shared_ptr<RGABuffer>>buffers{};
	inline std::shared_ptr<RGABuffer>Select(){
		current++;
		current%=count;
		return buffers[current];
	}
	void CreateAll(int dmabuf_dev_fd);
	void DestroyAll();
	inline ~RGABuffers(){DestroyAll();}
};

struct RGAOperation{
	RgaAction action=RGA_NONE;
	RGABuffers buffers{};
	RGAImageDesc input{},output{};
	struct{
		int thickness=0;
		int trans_x=0,trans_y=0;
		int border_top=0,border_bottom=0;
		int border_left=0,border_right=0;
		int border_value=0;
		double factor_x=0,factor_y=0;
		IM_USAGE mode_rotation=IM_HAL_TRANSFORM_ROT_270;
		IM_USAGE mode_flip=IM_HAL_TRANSFORM_FLIP_H_V;
		IM_USAGE mode_colorkey=IM_ALPHA_COLORKEY_NORMAL;
		std::vector<im_rect>rects;
		im_color color;
		im_colorkey_range range;
		IM_ROP_CODE rop;
		IM_BORDER_TYPE border=IM_BORDER_CONSTANT;
		IM_MOSAIC_MODE mosaic=IM_MOSAIC_8;
		IM_SCALE_MODE interpolation=INTER_LINEAR;
		IM_COLOR_SPACE_MODE colorspace=IM_COLOR_SPACE_DEFAULT;
	}options{};
	void ParseConfig(YAML::Node&cfg);
	std::shared_ptr<RGABuffer>DoAction(
		std::shared_ptr<RGABuffer>input,
		bool ro_in=true
	);
};

class RockchipRasterGraphicAcceleration:public Stream{
	public:
		~RockchipRasterGraphicAcceleration()override=default;
		explicit RockchipRasterGraphicAcceleration(webrtc_kvm*ctx);
		[[nodiscard]] inline std::string GetDriverName()const final{return "rkrga";}
		[[nodiscard]] inline StreamType GetType()const override{return STREAM_PIPE;}
		[[nodiscard]] inline uint32_t GetWidth()const override{return output.width;}
		[[nodiscard]] inline uint32_t GetHeight()const override{return output.height;}
		[[nodiscard]] inline uint32_t GetFrameRate()const override{return fps;}
	protected:
		void OnInitialize()final;
		void OnDeinitialize()final;
		void OnStartStream()final;
		void OnStopStream()final;
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		void OnProcessInput(StreamBuffer*buffer)final;
		void CreateBuffer(RGABuffer&b,int fd);
		void DestroyBuffer(RGABuffer&b);
		RGAImageDesc input{},output{};
		uint32_t fps=30;
		uint64_t frames=0;
		std::vector<std::shared_ptr<RGAOperation>>operations{};
		std::string dmabuf_dev="/dev/dma_heap/system-uncached-dma32";
};

extern bool StringToRgaAction(const std::string&val,RgaAction&action);
extern bool StringToRgaRopCode(const std::string&val,IM_ROP_CODE&rop);
extern bool StringToRgaBorderType(const std::string&val,IM_BORDER_TYPE&border);
extern uint32_t RgaFormatToFourcc(RgaSURF_FORMAT fmt);
extern RgaSURF_FORMAT FourccToRgaFormat(uint32_t fourcc);
#endif
