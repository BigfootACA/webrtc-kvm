/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"stdbool.h"
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#if defined(__arm__)&&__SIZEOF_POINTER__==4
#define __USE_TIME_BITS64
#endif
#ifndef __cplusplus
#if __STDC_VERSION__>=201112L
#define static_assert _Static_assert
#else
#define static_assert(...)
#endif
#endif
static_assert(sizeof(char)==1,       "char is not 8-bit");
static_assert(sizeof(short)==2,      "short is not 16-bit");
static_assert(sizeof(int)==4,        "int is not 32-bit");
static_assert(sizeof(long long)==8,  "long is not 64-bit");
#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error byte order is not little endian
#endif
