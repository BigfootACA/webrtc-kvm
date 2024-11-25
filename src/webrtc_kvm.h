/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef WEBRTC_KVM_H
#define WEBRTC_KVM_H
#include<map>
#include<ctime>
#include<string>
#include<cerrno>
#include<fcntl.h>
#include<cstdio>
#include<climits>
#include<unistd.h>
#include<cstring>
#include<cstdlib>
#include<cstdint>
#include<cassert>
#include<functional>
#include<sys/stat.h>
#include<sys/ioctl.h>
#include<sys/epoll.h>
#include"defines.h"

#define ERROR_MAX 5

struct MHD_Daemon;
struct gadget_ctx;
class HttpAuthorization;
class InputContext;
class StreamList;
class EventLoop;

struct webrtc_kvm_usb{
	InputContext*input;
	gadget_ctx*gadget;
	bool enable;
	std::string udc;
	std::string name;
	std::string manufacturer;
	std::string product;
	uint16_t id_vendor;
	uint16_t id_product;
};

struct webrtc_kvm_video{
	uint32_t width,height,fps;
	bool auto_stop;
};

struct webrtc_kvm_http_auth_pam{
	bool enabled;
	bool show_user;
	std::string default_user;
	std::string service;
	std::vector<std::string>allowed;
};

struct webrtc_kvm_http_auth_password{
	bool enabled;
	bool show_user;
	std::string default_user;
	std::map<std::string,std::string>users;
};

struct webrtc_kvm_http_auth{
	bool no_auth;
	webrtc_kvm_http_auth_pam pam;
	webrtc_kvm_http_auth_password pwd;
	HttpAuthorization*auth;
};

struct webrtc_kvm_http{
	uint16_t port;
	MHD_Daemon*httpd;
	webrtc_kvm_http_auth auth;
};

struct webrtc_kvm{
	webrtc_kvm_usb usb;
	webrtc_kvm_video video;
	webrtc_kvm_http http;
	StreamList*streams;
	int event_fd;
	pthread_t rate_tid;
	EventLoop*loop;
	volatile uint32_t dog;
	volatile bool running:1;
};

struct stream_callback{
	std::function<int(const void*data,size_t len)>callback;
};

extern int RunUnitTest();
extern void SetRunning(webrtc_kvm*ctx,bool running);
extern void InitializeHTTP(webrtc_kvm*ctx);
extern void InitializeMain(webrtc_kvm*ctx);
extern void InitializeInput(webrtc_kvm*ctx);
extern void InitializeGadget(webrtc_kvm*ctx);
extern void InitializeRateThread(webrtc_kvm*ctx);
extern void ConfigPopulateDefault(webrtc_kvm*ctx);
extern bool ConfigLoadFile(webrtc_kvm*ctx,const char*path);
extern int xioctl_impl(int fd,unsigned long req,const char*name,unsigned long val);
#define xioctl(fd,request,v) xioctl_impl(fd,request,#request,(unsigned long)v)
#endif
