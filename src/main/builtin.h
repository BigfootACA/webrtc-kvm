/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#if __SIZEOF_POINTER__==4
#define ASM_PTR_NUM .long
#elif __SIZEOF_POINTER__==8
#define ASM_PTR_NUM .quad
#else
#error unknown pointer size
#endif
#define DECL_ASM_SYM(_name,_type) _name:.globl _name;.type _name, %_type
#define DECL_ASM_FUNC(_name) DECL_ASM_SYM(_name,function)
#define DECL_ASM_VAR(_name) DECL_ASM_SYM(_name,object)
#define DECL_ASM_END(_name) .size _name, (. - _name)
#define DECL_ASM_VAR8(_name,_val)   DECL_ASM_VAR(_name);.byte  _val;DECL_ASM_END(_name)
#define DECL_ASM_VAR16(_name,_val)  DECL_ASM_VAR(_name);.short _val;DECL_ASM_END(_name)
#define DECL_ASM_VAR32(_name,_val)  DECL_ASM_VAR(_name);.long  _val;DECL_ASM_END(_name)
#define DECL_ASM_VAR64(_name,_val)  DECL_ASM_VAR(_name);.quad  _val;DECL_ASM_END(_name)
#define DECL_ASM_VARN(_name,_val)   DECL_ASM_VAR(_name);ASM_PTR_NUM  _val;DECL_ASM_END(_name)
#define DECL_ASM_STRING(_name,_val) DECL_ASM_VAR(_name);.ascii _val;DECL_ASM_END(_name)
#define DECL_ASM_SPACE(_name,_len)  DECL_ASM_VAR(_name);.space _len;DECL_ASM_END(_name)
#define DECL_BIN(_name,_path) \
	DECL_ASM_VAR(bin_##_name##_start); \
		.incbin _path; \
	DECL_ASM_END(bin_##_name##_start); \
	DECL_ASM_VAR(bin_##_name##_end); \
	.balign 8; \
	DECL_ASM_VAR64(bin_##_name##_size,bin_##_name##_end - bin_##_name##_start)
#if defined(__cplusplus)||defined(__STDC_VERSION__)
#include<stdint.h>
struct file_item{
	const char*name;
	const uint8_t*data;
	const uint64_t len;
};
#endif
