/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"screen.h"
#include"lib/log.h"

void WebRTCScreen::Handshake(){
	auto on_failed=[this](const std::string&reason){
		setStatus(Disconnected);
		auto s=std::format("WebRTC handshake failed: {}",reason);
		QMessageBox::critical(this,"Connect failed",QString::fromStdString(s));
	};
	auto on_request_failed=[on_failed](auto x,auto,auto r){
		on_failed(r);
	};
	auto on_get_sdp=[this](auto x,auto){
		auto sdp=x["sdp"].asString();
		log_info("webrtc got sdp {} bytes",sdp.size());
		auto dsc=rtc::Description(sdp,rtc::Description::Type::Offer);
		pc->setRemoteDescription(dsc);
	};
	auto on_new=[this,on_get_sdp,on_request_failed](auto x,auto){
		auto id=UUID(x["id"].asString());
		log_info("webrtc id: {}",id.ToString());
		webrtc_id=id;
		Json::Value req;
		req["id"]=id.ToString();
		HttpCall("/api/webrtc/get_sdp",req,on_get_sdp,on_request_failed);
	};
	setStatus(Connecting);
	HttpCall("/api/webrtc/new",{},on_new,on_request_failed);
}
