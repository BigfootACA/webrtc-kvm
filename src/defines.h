/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef DEFINES_H
#define DEFINES_H
#define BIT(nr) (1ULL<<(nr))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define NEG(r) (((r)>0)?(-r):(r))
#define EGO(expr,tag)do{expr;goto tag;}while(0)
#define EDONE(expr) EGO(expr,done)
#define SETERR(err) ((errno)=(err))
#define SETXERR(err) SETERR(errno?:(err))
#define ERRDONE(err) EDONE(SETERR(err))
#define PSETERR(err) ({SETERR(err);NULL;})
#define ENUM(err) -SETERR(err)
#define ERET(err) do{return ENUM(err);}while(0)
#define XERET(err) do{return -SETXERR(err);}while(0)
#define EPRET(err) do{SETERR(err);return NULL;}while(0)
#define RET(e) do{return SETERR(e);}while(0)
#define DONE(e) EDONE(SETERR(e))
#define XDONE(e) EDONE(SETXERR(e))
#define XRET(e,d) do{return SETERR((e)?:(d));}while(0)
#define EXRET(e) do{return SETXERR(e);}while(0)
#define EXPRET(e) do{SETXERR(e);return NULL;}while(0)
#define BOOL2STR(b) ((b)?"true":"false")
#define ARRAY_SIZE(array) (sizeof(array)/ sizeof((array)[0]))
#define header_func static inline __attribute__((unused))
#define timespec_diff(tb,ta) ((ta.tv_sec*1000000000UL+ta.tv_nsec)-(tb.tv_sec*1000000000UL+tb.tv_nsec))
#define ret_printf(e,val...) do{fprintf(stderr,val);return (e);}while(0)
#define set_errno(e) ({errno=(e);-(e);})
#define goto_errno(val) ({errno=(val);goto error;})
#define goto_printf(val...) ({fprintf(stderr,val);goto error;})
#define round_up(val,alg) (((val)+(alg)-1)&~((alg)-1))
#define round_down(val,alg) ((val)&~((alg)-1))
#define cdecl_attr(attr...) __attribute__((attr))
#define cdecl_attr_visibility(visibility) cdecl_attr(__visibility__(#visibility))
#define cdecl_attr_msabi cdecl_attr(ms_abi)
#define cdecl_attr_hidden cdecl_attr_visibility(hidden)
#define cdecl_attr_used cdecl_attr(used)
#define cdecl_attr_unused cdecl_attr(unused)
#define cdecl_attr_packed cdecl_attr(packed)
#define cdecl_attr_constructor cdecl_attr(constructor)
#define cdecl_attr_no_stack_chk cdecl_attr(no_stack_protector)
#define cdecl_attr_printf(fmt,va) cdecl_attr(format(printf,fmt,va))
#if 1
#define likely(x) (!!(x))
#define unlikely(x) (!!(x))
#else
#define likely(x) __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)
#endif
#define set_bit(var,val,bit)do{\
	if(bit)(var)=((typeof(var))((var)|(val)));\
	else (var)=((typeof(var))((var)&(~(val))));\
}while(0)
#define have_bit(var,bit)(((var)&(bit))==(bit))
typedef int runnable_t(void*);
#endif
