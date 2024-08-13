/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"configs.h"
#include"video/stream.h"
#include"lib/exception.h"
#include"lib/log.h"

static void load_video(webrtc_kvm*ctx,YAML::Node obj){
	if(!obj)return;
	if(auto v=obj["width"])ctx->video.width=v.as<uint32_t>();
	if(auto v=obj["height"])ctx->video.height=v.as<uint32_t>();
	if(auto v=obj["fps"])ctx->video.fps=v.as<uint32_t>();
	if(auto v=obj["auto_stop"])ctx->video.auto_stop=v.as<bool>();
	if(auto streams=obj["streams"])StreamFactory::CreateStreamsFromConfig(ctx,ctx->streams,streams);
	if(auto links=obj["links"])StreamLink::CreateLinksFromConfig(ctx->streams,links);
}

static void load_usb(webrtc_kvm*ctx,YAML::Node obj){
	if(!obj)return;
	if(auto v=obj["udc"])ctx->usb.udc=v.as<std::string>();
	if(auto v=obj["gadget"])ctx->usb.name=v.as<std::string>();
	if(auto v=obj["manufacturer"])ctx->usb.manufacturer=v.as<std::string>();
	if(auto v=obj["product"])ctx->usb.product=v.as<std::string>();
	if(auto v=obj["id_product"])ctx->usb.id_product=v.as<uint16_t>();
	if(auto v=obj["id_vendor"])ctx->usb.id_vendor=v.as<uint16_t>();
}

static void load_http_auth_pam(webrtc_kvm*ctx,YAML::Node obj){
	if(!obj)return;
	auto&p=ctx->http.auth.pam;
	if(auto v=obj["username"])p.show_user=v.as<bool>();
	if(auto v=obj["default"])p.default_user=v.as<std::string>();
	if(auto v=obj["service"])p.service=v.as<std::string>();
	if(auto v=obj["allowed"])for(const auto&i:v)
		p.allowed.push_back(i.as<std::string>());
}

static void load_http_auth_password(webrtc_kvm*ctx,YAML::Node obj){
	if(!obj)return;
	auto&p=ctx->http.auth.pwd;
	if(auto v=obj["username"])p.show_user=v.as<bool>();
	if(auto v=obj["default"])p.default_user=v.as<std::string>();
	if(auto v=obj["users"])for(const auto&i:v)
		p.users[i.first.as<std::string>()]=i.second.as<std::string>();
}

static void load_http(webrtc_kvm*ctx,YAML::Node obj){
	if(!obj)return;
	if(auto v=obj["port"])ctx->http.port=v.as<uint16_t>();
	if(auto v=obj["auth"]){
		auto method=v.as<std::string>();
		ctx->http.auth.no_auth=false;
		ctx->http.auth.pam.enabled=false;
		ctx->http.auth.pwd.enabled=false;
		if(method=="none")ctx->http.auth.no_auth=true;
		else if(method=="pam"){
			ctx->http.auth.pam.enabled=true;
			load_http_auth_pam(ctx,obj["pam"]);
		}else if(method=="password"){
			ctx->http.auth.pwd.enabled=true;
			load_http_auth_password(ctx,obj["password"]);
		}else throw InvalidArgument("unknown auth method {}",method);
	}
}

void ConfigLoadYAML(webrtc_kvm*ctx,YAML::Node obj){
	if(!obj)return;
	load_usb(ctx,obj["usb"]);
	load_http(ctx,obj["http"]);
	load_video(ctx,obj["video"]);
}

bool ConfigLoadFile(webrtc_kvm*ctx,const char*path){
	try{
		ConfigLoadYAML(ctx,YAML::LoadFile(path));
	}catch(std::exception&exc){
		log_err("failed to load {}: {}",path,exc.what());
		return false;
	}
	return true;
}
