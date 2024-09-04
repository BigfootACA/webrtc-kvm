/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"screen.h"
#include"lib/readable.h"
#include"lib/log.h"

void WebRTCScreen::enterEvent(QEnterEvent*event){
	// log_info("got focus");
	video->setFocus();
}

void WebRTCScreen::resizeEvent(QResizeEvent*event){
	QMainWindow::resizeEvent(event);
	auto cs=central->size();
	auto ws=event->size();
	auto ow=ws.width()-cs.width();
	auto oh=ws.height()-cs.height();
	if(ow!=outbox_size.width()||oh!=outbox_size.height()){
		outbox_size.setWidth(ow);
		outbox_size.setHeight(oh);
		ChooseScaleFactor();
	}
	onResizeVideo(event);
}

void WebRTCScreen::onActionConnectTriggered(bool){
	Connect();
}

void WebRTCScreen::onActionDisconnectTriggered(bool){
	Disconnect();
}

void WebRTCScreen::onActionLogoutTriggered(bool){
	Logout();
}

void WebRTCScreen::onScaleFactorTriggered(int factor){
	for(auto action:action_scale_factor){
		auto f=action->data().toInt();
		action->setChecked(f==factor);
	}
	if(scale_factor!=factor){
		scale_factor_changed=true;
		scale_factor=factor;
	}
	ChooseScaleFactor();
}

void WebRTCScreen::onTimerTimeout(){
	WebRTCScreen::Counter c{};
	c.frames=current.frames-last.frames;
	c.packets=current.packets-last.packets;
	c.bytes=current.bytes-last.bytes;
	last=current;
	status_frames->setText(
		QString("%0 FPS / %1 frames")
		.arg(c.frames)
		.arg(current.frames)
	);
	status_packets->setText(
		QString("%0 pps / %1 packets")
		.arg(c.packets)
		.arg(current.packets)
	);
	status_bytes->setText(
		QString("%0 / %1")
		.arg(QString::fromStdString(FormatSizeFloat(c.bytes,size_units_ibs)))
		.arg(QString::fromStdString(SizeStringFloat(current.bytes)))
	);
}

void WebRTCScreen::onJob(std::function<void()>job){
	if(job)job();
}

void WebRTCScreen::SubmitJob(std::function<void()>job,Qt::ConnectionType type){
	QMetaObject::invokeMethod(this,"onJob",type,Q_ARG(std::function<void()>,job));
}
