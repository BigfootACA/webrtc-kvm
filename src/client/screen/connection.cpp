/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"screen.h"
#include"lib/log.h"

void WebRTCScreen::Connect(){
	if(status!=Disconnected)return;
	auto ret_server=QInputDialog::getText(
		this,"Connect to server","Input server address",
		QLineEdit::Normal,QString::fromStdString(server)
	);
	if(ret_server.isEmpty())return;
	settings.setValue("remote/server",ret_server);
	server=ret_server.toStdString();
	log_info("use server {}",server);
	setStatus(Authenticating);
	TryLogin([this](){
		InitWebRTC();
		Handshake();
	},[this](const std::string&reason){
		setStatus(Disconnected);
		auto s=std::format("Login failed: {}",reason);
		QMessageBox::critical(this,"Login failed",QString::fromStdString(s));
	});
}

void WebRTCScreen::Disconnect(){
	if(status!=Connected)return;
	if(pc)pc->close();
}
