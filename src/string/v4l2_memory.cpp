/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"strings.h"
#include<string>

bool StringToV4L2Memory(const std::string&val,v4l2_memory&mem){
	if(val=="mmap")mem=V4L2_MEMORY_MMAP;
	else if(val=="userptr")mem=V4L2_MEMORY_USERPTR;
	else if(val=="overlay")mem=V4L2_MEMORY_OVERLAY;
	else if(val=="dmabuf")mem=V4L2_MEMORY_DMABUF;
	else return false;
	return true;
}
