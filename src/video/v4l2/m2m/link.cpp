/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"

void V4L2MemoryToMemory::OnBindInput(std::shared_ptr<StreamLink>link){
	out->OnBindInput(link);
	out->input=link;
}

void V4L2MemoryToMemory::OnBindOutput(std::shared_ptr<StreamLink>link){
	cap->OnBindOutput(link);
	cap->output=link;
}
