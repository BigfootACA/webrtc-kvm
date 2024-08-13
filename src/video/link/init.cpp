/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/exception.h"
#include"config/configs.h"

void StreamLink::Initialize(Stream*stream_source,Stream*stream_sink,BufferType buff_type){
	BindSource(stream_source);
	BindSink(stream_sink);
	this->type=buff_type;
	source->OnBindOutput(shared_from_this());
	sink->OnBindInput(shared_from_this());
}

static bool StringToBufferType(const std::string&val,BufferType&type){
	if(val=="pointer")type=BUFFER_POINTER;
	else if(val=="dmabuf")type=BUFFER_DMABUF;
	else return false;
	return true;
}

void StreamLink::Initialize(StreamList*list,YAML::Node&cfg){
	Stream*stream_source=nullptr,*stream_sink=nullptr;
	if(auto val_source=cfg["source"]){
		auto id=val_source.as<std::string>();
		stream_source=list->FindByID(id);
		if(!stream_source)throw InvalidArgument("source {} not found",id);
	}
	if(auto val_sink=cfg["sink"]){
		auto id=val_sink.as<std::string>();
		stream_sink=list->FindByID(id);
		if(!stream_sink)throw InvalidArgument("sink {} not found",id);
	}
	if(!stream_source||!stream_sink)throw InvalidArgument("link incomplete");
	if(stream_source==stream_sink)throw InvalidArgument("link loop detected");
	BufferType buff_type=BUFFER_NULL;
	load_string_conv(cfg,type,buff_type,BufferType);
	Initialize(stream_source,stream_sink,buff_type);
	if(auto val_size=cfg["size"]){
		if(val_size.IsScalar())planes[0].size=val_size.as<size_t>();
		else if(val_size.IsSequence())for(size_t i=0;i<val_size.size();i++)
			if(i<ARRAY_SIZE(planes))planes[i].size=val_size[i].as<size_t>();
	}
}
