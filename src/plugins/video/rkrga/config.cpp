/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkrga.h"
#include"config/configs.h"
#include"lib/log.h"

static RgaSURF_FORMAT LoadFormat(YAML::Node&node){
	uint32_t fourcc=0;
	auto value=node.as<std::string>();
	if(!StringToFourcc(value,fourcc))
		throw RuntimeError("bad fourcc {}",value);
	return FourccToRgaFormat(fourcc);
}

static void LoadRect(YAML::Node&node,im_rect&rect){
	if(auto v=node["x"])rect.x=v.as<int>();
	if(auto v=node["y"])rect.y=v.as<int>();
	if(auto v=node["w"])rect.width=v.as<int>();
	if(auto v=node["h"])rect.height=v.as<int>();
	if(auto v=node["width"])rect.width=v.as<int>();
	if(auto v=node["height"])rect.height=v.as<int>();
}

static void LoadRects(YAML::Node&node,std::vector<im_rect>&rects){
	auto load=[&](YAML::Node&n){
		im_rect rect{};
		LoadRect(n,rect);
		rects.push_back(rect);
	};
	if(node.IsMap())
		load(node);
	else if(node.IsSequence())
		for(auto sub:node)
			load(sub);
}

bool RGAImageDesc::Empty(){
	if(width==0)return true;
	if(height==0)return true;
	if(format==RK_FORMAT_UNKNOWN)return true;
	return false;
}

void RGAImageDesc::FillMissing(const RGAImageDesc&desc){
	if(width==0&&desc.width!=0)width=desc.width;
	if(height==0&&desc.height!=0)height=desc.height;
	if(format==RK_FORMAT_UNKNOWN&&desc.format!=RK_FORMAT_UNKNOWN)format=desc.format;
	Update();
}

void RGAImageDesc::Update(){
	if(auto_size&&width!=0&&height!=0&&format!=RK_FORMAT_UNKNOWN)
		size=width*height*get_bpp_from_format(format);
}

void RGAImageDesc::ParseConfig(YAML::Node&cfg){
	if(auto v=cfg["width"])width=v.as<int>();
	if(auto v=cfg["height"])height=v.as<int>();
	if(auto v=cfg["from"])format=LoadFormat(v);
	if(auto v=cfg["size"])auto_size=false,size=v.as<size_t>();
	Update();
}

void RGAOperation::ParseConfig(YAML::Node&cfg){
	load_string_conv(cfg,action,action,RgaAction);
	if(auto v=cfg["buffers"])buffers.count=v.as<size_t>();
	if(auto v=cfg["input"])input.ParseConfig(v);
	if(auto v=cfg["output"])output.ParseConfig(v);
	if(buffers.count<=0)
		throw InvalidArgument("bad buffers count");
	switch(action){
		case RGA_COPY:break;
		case RGA_RESIZE:
			if(auto v=cfg["x"])options.factor_x=v.as<double>();
			if(auto v=cfg["y"])options.factor_y=v.as<double>();
		break;
		case RGA_CROP:
			if(auto v=cfg["rect"]){
				options.rects.clear();
				options.rects.push_back({});
				auto&r=options.rects[0];
				LoadRect(v,r);
				if(output.width==0)output.width=r.width;
				if(output.height==0)output.height=r.height;
			}
		break;
		case RGA_TRANSLATE:
			if(auto v=cfg["x"])options.trans_x=v.as<int>();
			if(auto v=cfg["y"])options.trans_y=v.as<int>();
		break;
		case RGA_CONVERT:
			if(auto v=cfg["from"])input.format=LoadFormat(v);
			if(auto v=cfg["to"])output.format=LoadFormat(v);
		break;
		case RGA_ROTATE:
			if(auto v=cfg["rotate"])switch(v.as<int>()){
				case 90:options.mode_rotation=IM_HAL_TRANSFORM_ROT_90;break;
				case 180:options.mode_rotation=IM_HAL_TRANSFORM_ROT_180;break;
				case 270:options.mode_rotation=IM_HAL_TRANSFORM_ROT_270;break;
				default:throw RuntimeError("bad rotate {}",v.as<int>());
			}
		break;
		case RGA_FILP:{
			bool h=false,v=false;
			if(auto v=cfg["horizontal"])h=v.as<bool>();
			if(auto v=cfg["vertical"])v=v.as<bool>();
			if(h&&v)options.mode_flip=IM_HAL_TRANSFORM_FLIP_H_V;
			else if(h)options.mode_flip=IM_HAL_TRANSFORM_FLIP_H;
			else if(v)options.mode_flip=IM_HAL_TRANSFORM_FLIP_V;
		}break;
		case RGA_COLORKEY:
			if(auto v=cfg["max"])options.range.max=v.as<int>();
			if(auto v=cfg["min"])options.range.min=v.as<int>();
			if(auto v=cfg["inverted"])if(v.as<bool>())
				options.mode_colorkey=IM_ALPHA_COLORKEY_INVERTED;
		break;
		case RGA_ROP:
			load_string_conv(cfg,rop,options.rop,RgaRopCode);
		break;
		case RGA_FILL:
			if(auto v=cfg["color"])options.color.value=v.as<uint32_t>();
			if(auto v=cfg["rect"])LoadRects(v,options.rects);
		break;
		case RGA_RECTANGLE:
			if(auto v=cfg["color"])options.color.value=v.as<uint32_t>();
			if(auto v=cfg["rect"])LoadRects(v,options.rects);
			if(auto v=cfg["thickness"])options.thickness=v.as<int>();
		break;
		case RGA_BORDER:
			if(auto v=cfg["top"])options.border_top=v.as<int>();
			if(auto v=cfg["bottom"])options.border_bottom=v.as<int>();
			if(auto v=cfg["left"])options.border_left=v.as<int>();
			if(auto v=cfg["right"])options.border_right=v.as<int>();
			if(auto v=cfg["value"])options.border_value=v.as<int>();
			load_string_conv(cfg,border,options.border,RgaBorderType);
		break;
		case RGA_MOSAIC:
			if(auto v=cfg["mosaic"])switch(v.as<int>()){
				case 8:options.mosaic=IM_MOSAIC_8;break;
				case 16:options.mosaic=IM_MOSAIC_16;break;
				case 32:options.mosaic=IM_MOSAIC_32;break;
				case 64:options.mosaic=IM_MOSAIC_64;break;
				case 128:options.mosaic=IM_MOSAIC_128;break;
				default:throw RuntimeError("bad mosaic {}",v.as<int>());
			}
		break;
		default:throw RuntimeError("unknown rga action");
	}
}

void RockchipRasterGraphicAcceleration::OnLoadDeviceConfig(YAML::Node&cfg){
	if(auto v=cfg["input"])input.ParseConfig(v);
	if(auto v=cfg["output"])output.ParseConfig(v);
	if(auto v=cfg["fps"])fps=v.as<uint32_t>();
	load_string_conv(cfg,fourcc,CurrentFourcc(),Fourcc);
	if(auto v=cfg["dmabuf"])dmabuf_dev=v.as<std::string>();
	if(auto v=cfg["operations"])for(auto op:v){
		auto oper=std::make_shared<RGAOperation>();
		oper->ParseConfig(op);
		operations.push_back(oper);
	}
	if(operations.size()<1)
		throw InvalidArgument("no any operations found");
}
