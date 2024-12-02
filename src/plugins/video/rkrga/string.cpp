/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"rkrga.h"

bool StringToRgaAction(const std::string&val,RgaAction&action){
	if(val=="copy")action=RGA_COPY;
	else if(val=="resize")action=RGA_RESIZE;
	else if(val=="crop")action=RGA_CROP;
	else if(val=="translate")action=RGA_TRANSLATE;
	else if(val=="convert")action=RGA_CONVERT;
	else if(val=="rotate")action=RGA_ROTATE;
	else if(val=="filp")action=RGA_FILP;
	else if(val=="colorkey")action=RGA_COLORKEY;
	else if(val=="rop")action=RGA_ROP;
	else if(val=="fill")action=RGA_FILL;
	else if(val=="rectangle")action=RGA_RECTANGLE;
	else if(val=="border")action=RGA_BORDER;
	else if(val=="mosaic")action=RGA_MOSAIC;
	else return false;
	return true;
}

bool StringToRgaRopCode(const std::string&val,IM_ROP_CODE&rop){
	if(val=="and")rop=IM_ROP_AND;
	else if(val=="or")rop=IM_ROP_OR;
	else if(val=="not-dst")rop=IM_ROP_NOT_DST;
	else if(val=="not-src")rop=IM_ROP_NOT_SRC;
	else if(val=="xor")rop=IM_ROP_XOR;
	else if(val=="not-xor")rop=IM_ROP_NOT_XOR;
	else return false;
	return true;
}

bool StringToRgaBorderType(const std::string&val,IM_BORDER_TYPE&border){
	if(val=="constant")border=IM_BORDER_CONSTANT;
	else if(val=="reflect")border=IM_BORDER_REFLECT;
	else if(val=="wrap")border=IM_BORDER_WRAP;
	else return false;
	return true;
}
