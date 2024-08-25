/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/log.h"
#include"lib/exception.h"
#include"string/strings.h"

void Stream::BindLink(std::shared_ptr<StreamLink>link,StreamLinkDirection dir){
	auto name=link->ToString();
	std::string type;
	uint32_t*fourcc;
	if(dir==LINK_DIR_IN)type="input",fourcc=&fourcc_in;
	else if(dir==LINK_DIR_OUT)type="output",fourcc=&fourcc_out;
	else throw InvalidArgument("unknown link direction");
	log_dbg(
		"bind {} {} {} with this {} link {}",
		GetID(),type,name,FourccToString(*fourcc),FourccToString(link->fourcc)
	);
	if(*fourcc==0&&link->fourcc!=0)*fourcc=link->fourcc;
	if(*fourcc!=0&&link->fourcc==0)link->fourcc=*fourcc;
	if(*fourcc==0&&link->fourcc==0)
		throw InvalidArgument("no fourcc in {} link {}",type,name);
	if(*fourcc!=link->fourcc)throw InvalidArgument(
		"fourcc mismatch in {} link {}: link {} != this {}",
		type,name,FourccToString(link->fourcc),FourccToString(*fourcc)
	);
	OnBindLink(link,dir);
}

size_t&Stream::OutputPlaneSize(size_t id,uint32_t plane){
	auto output=GetOutput(id);
	if(!output)throw InvalidArgument("no output link");
	if(plane>=ARRAY_SIZE(output->planes))
		throw InvalidArgument("plane index out of range");
	return output->planes[plane].size;
}

size_t&Stream::InputPlaneSize(uint32_t plane){
	if(!input)throw InvalidArgument("no input link");
	if(plane>=ARRAY_SIZE(input->planes))
		throw InvalidArgument("plane index out of range");
	return input->planes[plane].size;
}

Stream*Stream::GetInputStream()const{
	if(unlikely(!input))throw InvalidArgument("no input link");
	auto source=input->GetSource();
	if(unlikely(!source))throw InvalidArgument("no source stream in input link");
	return source;
}

Stream*Stream::GetOutputStream(size_t id)const{
	auto output=GetOutput(id);
	if(unlikely(!output))throw InvalidArgument("no output link");
	auto sink=output->GetSink();
	if(unlikely(!sink))throw InvalidArgument("no sink stream in output link");
	return sink;
}

std::shared_ptr<StreamLink>Stream::GetOutput(size_t id)const{
	if(id>=outputs.size())return nullptr;
	return outputs[id];
}
