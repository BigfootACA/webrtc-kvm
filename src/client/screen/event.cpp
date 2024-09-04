/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"screen.h"
#include"lib/log.h"
#include"lib/exception.h"
#include<QApplication>

void WebRTCScreen::onEventChannel(std::shared_ptr<rtc::DataChannel>ch){
	event_ch=ch;
	ch->onOpen([](){log_info("event channel opened");});
	ch->onClosed([](){log_info("event channel closed");});
	ch->onError([](auto err){log_warn("event channel error: {}",err);});
	ch->onMessage([this](rtc::message_variant m){
		Json::Reader rd;
		Json::Value node;
		try{
			auto data=std::get<rtc::string>(m);
			current.bytes+=data.size();
			current.packets+=1;
			if(!rd.parse(data,node))throw RuntimeError(
				"parse event data failed: {}",
				rd.getFormattedErrorMessages()
			);
			auto t=node["type"];
			if(!t.isString())throw InvalidArgument("bad event message");
			log_info("receive event {}",t.asString());
			QMetaObject::invokeMethod(this,"onEventMessage",node);
		}catch(std::exception&exc){
			log_info("process event failed: {}",exc.what());
		}
	});
}

void WebRTCScreen::onEventMessage(Json::Value msg){
	auto type=msg["type"].asString();
	if(type=="resize")onEventResizeMessage(msg);
}

void WebRTCScreen::ChooseScaleFactor(){
	auto width=video_size.width();
	auto height=video_size.height();
	auto pad_width=outbox_size.width();
	auto pad_height=outbox_size.height();
	auto rec=QApplication::primaryScreen()->availableGeometry();
	if(!scale_factor_changed){
		int choose=0,min_factor=INT_MAX;
		for(auto factor:scale_factors){
			auto scaled_width=width*factor/100;
			auto scaled_height=height*factor/100;
			if(min_factor>factor)min_factor=factor;
			if(scaled_width+pad_width>rec.width())continue;
			if(scaled_height+pad_height>rec.height())continue;
			if(choose==0||factor>choose)choose=factor;
		}
		if(choose==0)choose=min_factor;
		if(choose<scale_factor){
			log_info("auto selected scale factor {}%",choose);
			scale_factor=choose;
		}
	}
	auto video_width=width*scale_factor/100;
	auto video_height=height*scale_factor/100;
	video->resize(video_width,video_height);
	auto win_width=video_width+pad_width;
	auto win_height=video_height+pad_height;
	resize(win_width,win_height);
}

void WebRTCScreen::onEventResizeMessage(Json::Value msg){
	auto width=msg["width"].asInt();
	auto height=msg["height"].asInt();
	auto fps=msg["fps"].asInt();
	log_info("video resize to {}x{}@{}hz",width,height,fps);
	video_size.setWidth(width);
	video_size.setHeight(height);
	ChooseScaleFactor();
}
