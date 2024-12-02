/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkmpp.h"

void RockchipMediaProcessPlatform::DecoderPreInit(){
	video_type=FourccToMppCodingType(fourcc_in);
	buff_fmt=FourccToMppFrameFormat(fourcc_out);
	ctx_type=MPP_CTX_DEC;
}

void RockchipMediaProcessPlatform::DecoderPostInit(){

}
