/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkrga.h"
#include"lib/log.h"
#include"lib/exception.h"

std::shared_ptr<RGABuffer>RGAOperation::DoAction(
	std::shared_ptr<RGABuffer>input,
	bool ro_in
){
	if(unlikely(!input))
		throw InvalidArgument("bad input");
	std::shared_ptr<RGABuffer>output;
	rga_buffer_t src_img=input->buffer;
	rga_buffer_t dst_img{};
	bool inplace=false;
	IM_STATUS ret=IM_STATUS_FAILED;
	switch(action){
		case RGA_FILL:
		case RGA_RECTANGLE:
		case RGA_MOSAIC:
			inplace=true;
		break;
		default:;
	}
	if(inplace&&!ro_in){
		output=input;
		dst_img=output->buffer;
	}else{
		output=buffers.Select();
		if(unlikely(!output||output==input))
			throw InvalidArgument("no more available buffer");
		dst_img=output->buffer;
		if(inplace&&(ret=imcopy(src_img,dst_img))!=IM_STATUS_SUCCESS)
			throw RuntimeError("rga copy image failed: {}",imStrError(ret));
	}
	switch(action){
		case RGA_COPY:
			ret=imcopy(src_img,dst_img);
		break;
		case RGA_RESIZE:
			if(options.factor_x==0&&options.factor_y==0){
				ret=imresize(src_img,dst_img);
			}else{
				ret=imresize(
					src_img,dst_img,
					options.factor_x,
					options.factor_y,
					options.interpolation
				);
			}
		break;
		case RGA_CROP:
			if(unlikely(options.rects.size()!=1))
				throw RuntimeError("bad rect count");
			ret=imcrop(src_img,dst_img,options.rects[0]);
		break;
		case RGA_TRANSLATE:
			ret=imtranslate(
				src_img,dst_img,
				options.trans_x,
				options.trans_y
			);
		break;
		case RGA_CONVERT:
			ret=imcvtcolor(
				src_img,dst_img,
				this->input.format,
				this->output.format,
				options.colorspace
			);
		break;
		case RGA_ROTATE:
			ret=imrotate(src_img,dst_img,options.mode_rotation);
		break;
		case RGA_FILP:
			ret=imflip(src_img,dst_img,options.mode_flip);
		break;
		case RGA_COLORKEY:
			ret=imcolorkey(
				src_img,dst_img,
				options.range,
				options.mode_colorkey
			);
		break;
		case RGA_ROP:
			ret=imrop(src_img,dst_img,options.rop);
		break;
		case RGA_FILL:
			ret=imfillArray(
				dst_img,
				(im_rect*)options.rects.data(),
				options.rects.size(),
				options.color.value
			);
		break;
		case RGA_RECTANGLE:
			ret=imrectangleArray(
				dst_img,
				(im_rect*)options.rects.data(),
				options.rects.size(),
				options.color.value,
				options.thickness
			);
		break;
		case RGA_BORDER:
			ret=immakeBorder(
				src_img,dst_img,
				options.border_top,
				options.border_bottom,
				options.border_left,
				options.border_right,
				options.border,
				options.border_value
			);
		break;
		case RGA_MOSAIC:
			if(unlikely(options.rects.size()!=1))
				throw RuntimeError("bad rect count");
			ret=immosaic(dst_img,options.rects[0],options.mosaic);
		break;
		default:;
	}
	if(ret!=IM_STATUS_SUCCESS)throw RuntimeError("rga action failed: {}",imStrError(ret));
	return output;
}

void RockchipRasterGraphicAcceleration::OnProcessInput(StreamBuffer*buffer){
	auto import=std::make_shared<RGABuffer>();
	import->desc=input;
	import->Import(buffer);
	size_t id=0;
	std::shared_ptr<RGABuffer>next=import;
	for(auto&oper:operations)try{
		auto cur=oper->DoAction(next,next==import);
		next=cur;
		id++;
	}catch(...){
		log_err("process image operation {} failed",id);
		throw;
	}
	ProcessNext([&](auto output,auto sink,auto process){
		if(!next||next==import)
			throw InvalidArgument("bad current output");
		StreamBuffer buf{};
		buf.type=output->GetType();
		buf.timestamp=buffer->timestamp;
		buf.planes_cnt=1;
		buf.planes[0].used=next->desc.size;
		buf.planes[0].size=next->desc.size;
		switch(buf.type){
			case BUFFER_DMABUF:buf.planes[0].fd=next->dmabuf_fd;break;
			case BUFFER_POINTER:buf.planes[0].ptr=next->pointer;break;
			default:throw RuntimeError("bad buffer type");
		}
		return process(&buf);
	});
	frames++;
}
