/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"v4l2_m2m.h"

void V4L2MemoryToMemory::OnInitializeEncoder(){
	v4l2_streamparm streamparm={
		.type=out->type,
		.parm={.output={.timeperframe={
			.numerator=out->GetFrameRate()*1000,
			.denominator=1000,
		}}}
	};
	v4l2_parm_set(device_fd,streamparm);
	if(out->GetWidth()!=width||out->GetHeight()!=height){
		log_info("output size mismatch, setup selection");
		log_info("crop {}x{} to {}x{}",out->GetWidth(),out->GetHeight(),width,height);
		v4l2_selection selection={
			.type=v4l2_type_base(out->type),
			.target=V4L2_SEL_TGT_CROP,
			.r={.left=0,.top=0,.width=width,.height=height},
		};
		v4l2_selection_set(device_fd,selection);
	}
}
