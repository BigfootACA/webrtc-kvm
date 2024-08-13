/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_dev.h"
#include"string/strings.h"

void V4L2Device::OnBindLink(std::shared_ptr<StreamLink>link,const std::string&type){
	auto name=link->ToString();
	log_dbg(
		"bind {} {} {} with this {} link {}",
		GetID(),type,name,FourccToString(fourcc),FourccToString(link->fourcc)
	);
	if(fourcc==0&&link->fourcc!=0)fourcc=link->fourcc;
	if(fourcc!=0&&link->fourcc==0)link->fourcc=fourcc;
	if(fourcc==0&&link->fourcc==0)
		throw InvalidArgument("no fourcc in {} link {}",type,name);
	if(fourcc!=link->fourcc)throw InvalidArgument(
		"fourcc mismatch in {} link {}: link {} != this {}",
		type,name,FourccToString(link->fourcc),FourccToString(fourcc)
	);
	if(memory==0)switch(link->type){
		case BUFFER_POINTER:memory=V4L2_MEMORY_MMAP;break;
		case BUFFER_DMABUF:memory=V4L2_MEMORY_DMABUF;break;
		default:;
	}
}
