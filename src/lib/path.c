/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 * Path process functions
 *
 */

#include<stddef.h>
#include<string.h>
#include<stdbool.h>
#include<limits.h>
#include<unistd.h>
#include"path.h"
#include"defines.h"

static inline bool is_sep(char c){return c=='/'||c=='\\';}

size_t path_resolve(char*buff,size_t size,char sep,const char*path){
	size_t len=0;
	if(unlikely(!buff||size<=0||!path))return 0;
	if(sep==0)sep='/';
	memset(buff,0,size);
	for(;size>0&&*path;path++){
		if(is_sep(path[0])){
			if(len>0&&buff[-1]!=sep)
				*(buff++)='/',len++,size--;
			continue;
		}
		if(path[0]=='.'&&(len<=0||is_sep(buff[-1]))){
			if(is_sep(path[1])){
				path++;
				continue;
			}
			if(path[1]=='.'&&is_sep(path[2])){
				if(len>0){
					if(buff[-1]==sep)
						*(--buff)=0,len--,size++;
					while(len>0&&buff[-1]!=sep)
						*(--buff)=0,len--,size++;
				}
				path+=2;
				continue;
			}
		}
		*(buff++)=*path,len++,size--;
	}
	return len;
}

size_t path_merge(char*buff,size_t size,char sep,const char*path1,const char*path2){
	char path[4000];
	if(unlikely(!buff||size<=0))return 0;
	if(!path1||!path2){
		if(path1)return path_resolve(buff,size,sep,path1);
		else if(path2)return path_resolve(buff,size,sep,path2);
		else return 0;
	}
	memset(path,0,sizeof(path));
	strlcat(path,path1,sizeof(path)-1);
	strlcat(path,"/",sizeof(path)-1);
	strlcat(path,path2,sizeof(path)-1);
	return path_resolve(buff,size,sep,path);
}

const char*path_basename(char*buff,size_t size,const char*path,size_t len){
	size_t cnt;
	const char*start,*end,*p;
	if(unlikely(!buff||size<=0||!path))return NULL;
	if(len<=0)len=strlen(path);
	memset(buff,0,size);
	start=path,end=start+len;
	while(end>start&&is_sep(end[-1]))end--;
	for(p=start;p<end;p++)if(is_sep(*p))start=p+1;
	cnt=MIN((size_t)(end-start),size);
	if(cnt>0)strncpy(buff,start,cnt);
	return buff;
}

const char*path_dirname(char*buff,size_t size,const char*path,size_t len){
	size_t cnt;
	const char*start,*end;
	if(unlikely(!buff||size<=0||!path))return NULL;
	if(len<=0)len=strlen(path);
	memset(buff,0,size);
	start=path,end=start+len;
	while(end>start&&is_sep(end[-1]))end--;
	while(end>start&&!is_sep(end[-1]))end--;
	cnt=MIN((size_t)(end-start),size);
	if(cnt>0)strncpy(buff,start,cnt);
	return buff;
}

const char*path_get_self(char*buff,size_t size){
	memset(buff,0,size);
	return likely(readlink("/proc/self/exe",buff,size)>0)?buff:NULL;
}

const char*path_get_self_folder(char*buff,size_t size){
	char tmp[PATH_MAX];
	if(unlikely(!path_get_self(tmp,sizeof(tmp))))return NULL;
	return path_dirname(buff,size,tmp,0);
}
