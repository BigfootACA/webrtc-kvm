/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"

#define MIME_ARGS0(t,...)
#define MIME_ARGS1(t,a0,...) {#a0,#t},
#define MIME_ARGS2(t,a0,a1,...) {#a0,#t},{#a1,#t},
#define MIME_ARGS3(t,a0,a1,a2,...) {#a0,#t},{#a1,#t},{#a2,#t},
#define MIME_ARGS4(t,a0,a1,a2,a3,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},
#define MIME_ARGS5(t,a0,a1,a2,a3,a4,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},
#define MIME_ARGS6(t,a0,a1,a2,a3,a4,a5,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},
#define MIME_ARGS7(t,a0,a1,a2,a3,a4,a5,a6,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},
#define MIME_ARGS8(t,a0,a1,a2,a3,a4,a5,a6,a7,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},
#define MIME_ARGS9(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},
#define MIME_ARGS10(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},{#a9,#t},
#define MIME_ARGS11(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},{#a9,#t},{#a10,#t},
#define MIME_ARGS12(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},{#a9,#t},{#a10,#t},{#a11,#t},
#define MIME_ARGS13(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},{#a9,#t},{#a10,#t},{#a11,#t},{#a12,#t},
#define MIME_ARGS14(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},{#a9,#t},{#a10,#t},{#a11,#t},{#a12,#t},{#a13,#t},
#define MIME_ARGS15(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},{#a9,#t},{#a10,#t},{#a11,#t},{#a12,#t},{#a13,#t},{#a14,#t},
#define MIME_ARGS16(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},{#a9,#t},{#a10,#t},{#a11,#t},{#a12,#t},{#a13,#t},{#a14,#t},{#a15,#t},
#define MIME_ARGS17(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},{#a9,#t},{#a10,#t},{#a11,#t},{#a12,#t},{#a13,#t},{#a14,#t},{#a15,#t},{#a16,#t},
#define MIME_ARGS18(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},{#a9,#t},{#a10,#t},{#a11,#t},{#a12,#t},{#a13,#t},{#a14,#t},{#a15,#t},{#a16,#t},{#a17,#t},
#define MIME_ARGS19(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},{#a9,#t},{#a10,#t},{#a11,#t},{#a12,#t},{#a13,#t},{#a14,#t},{#a15,#t},{#a16,#t},{#a17,#t},{#a18,#t},
#define MIME_ARGS20(t,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,...) {#a0,#t},{#a1,#t},{#a2,#t},{#a3,#t},{#a4,#t},{#a5,#t},{#a6,#t},{#a7,#t},{#a8,#t},{#a9,#t},{#a10,#t},{#a11,#t},{#a12,#t},{#a13,#t},{#a14,#t},{#a15,#t},{#a16,#t},{#a17,#t},{#a18,#t},{#a19,#t},
#define MIME_ARGS_(t,cnt,...)MIME_ARGS##cnt(t,__VA_ARGS__)
#define MIME_ARGS(t,cnt,...)MIME_ARGS_(t,cnt,__VA_ARGS__)
#define ARGS_CNT_E(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,cnt,...)cnt
#define ARGS_CNT(...) ARGS_CNT_E(__VA_ARGS__,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

static const struct mime_type{
	const char*ext;
	const char*mime;
}mime_list[]={
	#define MIME(mime,ext...) MIME_ARGS(mime,ARGS_CNT(ext),ext)
	#include"mime.h"
	#undef MIME
	{nullptr,nullptr}
};
struct MimeTypes{
	std::map<std::string,std::string>m;
};
static MimeTypes*mime_map=nullptr;
static const char*fallback="application/octet-stream";

static void InitializeMime(){
	mime_map=new MimeTypes;
	for(size_t i=0;mime_list[i].ext;i++)
		mime_map->m[mime_list[i].ext]=mime_list[i].mime;
}

std::string FileNameToMime(const std::string&name){
	auto dot=name.rfind('.');
	if(dot==std::string::npos)return fallback;
	auto ext=name.substr(dot+1);
	if(unlikely(!mime_map))InitializeMime();
	auto e=mime_map->m.find(ext);
	if(e==mime_map->m.end())return fallback;
	return e->second;
}
