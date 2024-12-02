/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include "lib/readable.h"
#include"rkrga.h"
#include"lib/log.h"
#include"lib/exception.h"
#include"abstract/resource_release.h"
#include "string/strings.h"

void RockchipRasterGraphicAcceleration::OnInitialize(){

}

void RockchipRasterGraphicAcceleration::OnDeinitialize(){

}

void RockchipRasterGraphicAcceleration::OnStartStream(){
	auto inp=GetInputStream();
	if((input.width=inp->GetWidth())==0||(input.height=inp->GetHeight())==0)
		throw InvalidArgument("no width or height");
	if(output.width==0)output.width=input.width;
	if(output.height==0)output.height=input.height;
	log_info("rkrga {} input format {}x{}@{}hz",GetID(),input.width,input.height,fps);
	log_info("rkrga {} output format {}x{}@{}hz",GetID(),output.width,output.height,fps);
	if((input.format=FourccToRgaFormat(fourcc_in))==RK_FORMAT_UNKNOWN)
		throw InvalidArgument("unsupported input fourcc");
	if((output.format=FourccToRgaFormat(fourcc_out))==RK_FORMAT_UNKNOWN)
		throw InvalidArgument("unsupported output fourcc");
	input.Update();
	output.Update();
	auto inp_size=InputPlaneSize(0);
	if(inp_size>input.size)input.size=inp_size;
	for(size_t i=0;i<outputs.size();i++)
		output.size=MAX(output.size,OutputPlaneSize(i,0));
	int fd=open(dmabuf_dev.c_str(),O_RDWR);
	if(fd<0)throw RuntimeError(
		"open dmabuf device {} failed: {}",
		dmabuf_dev,strerror(errno)
	);
	FDRelease dmabuf_f(fd);
	std::shared_ptr<RGAOperation>last=nullptr;
	for(auto op:operations){
		if(last)last->output.FillMissing(last->input);
		op->input.FillMissing(last?last->output:input);
		last=op;
		op->buffers.desc=output;
		op->buffers.CreateAll(fd);
	}
	if(last)last->output=output;
	for(auto op:operations){
		op->buffers.desc=op->output;
		op->buffers.CreateAll(fd);
	}
}

void RockchipRasterGraphicAcceleration::OnStopStream(){
	for(auto op:operations)
		op->buffers.DestroyAll();
}
