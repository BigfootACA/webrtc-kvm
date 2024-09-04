/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"screen.h"
#include"lib/log.h"

WebRTCScreen::WebRTCScreen(
	QWidget*parent
):QMainWindow(parent){
	central=new QWidget(this);
	// auto main=new QVBoxLayout(central);
	setCentralWidget(central);

	/* Menu bar */
	menu_bar=new QMenuBar(this);
	setMenuBar(menu_bar);

	/* Menu bar: Connection */
	menu_connection=new QMenu(menu_bar);
	menu_bar->addAction(menu_connection->menuAction());

	/* Menu bar: Connection: Connect */
	action_connect=new QAction(this);
	connect(action_connect,&QAction::triggered,this,&WebRTCScreen::onActionConnectTriggered);
	menu_connection->addAction(action_connect);

	/* Menu bar: Connection: Disconnect */
	action_disconnect=new QAction(this);
	menu_connection->addAction(action_disconnect);
	connect(action_disconnect,&QAction::triggered,this,&WebRTCScreen::onActionDisconnectTriggered);

	/* Menu bar: Connection: Logout */
	action_logout=new QAction(this);
	menu_connection->addAction(action_logout);
	connect(action_logout,&QAction::triggered,this,&WebRTCScreen::onActionLogoutTriggered);

	/* Menu bar: Scale Factor */
	menu_scale=new QMenu(menu_bar);
	menu_bar->addAction(menu_scale->menuAction());

	for(size_t i=0;i<ARRAY_SIZE(scale_factors);i++){
		auto factor=scale_factors[i];
		auto a=new QAction(this);
		a->setText(QString("%0%").arg(factor));
		a->setCheckable(true);
		a->setData(factor);
		a->setChecked(scale_factor==factor);
		connect(a,&QAction::triggered,this,[this,factor](bool){
			onScaleFactorTriggered(factor);
		});
		menu_scale->addAction(a);
		action_scale_factor[i]=a;
	}

	/* Status bar */
	status_bar=new QStatusBar(this);
	setStatusBar(status_bar);

	/* Status bar: conection status */
	status_status=new QLabel(this);
	status_bar->addPermanentWidget(status_status);

	/* Status bar: Framerate / Frames counter */
	status_frames=new QLabel(this);
	status_bar->addPermanentWidget(status_frames);

	/* Status bar: Packets */
	status_packets=new QLabel(this);
	status_bar->addPermanentWidget(status_packets);

	/* Status bar: Network traffic meter */
	status_bytes=new QLabel(this);
	status_bar->addPermanentWidget(status_bytes);

	/* Video display */
	central->setStyleSheet("background-color:black");
	video=new VideoBox(central);

	/* Status Bar monitor */
	timer=new QTimer(this);
	connect(timer,&QTimer::timeout,this,&WebRTCScreen::onTimerTimeout);
	timer->start(1000);

	/* Translate text */
	Retranslate();
	setStatus(Disconnected);

	video_size.setWidth(600);
	video_size.setHeight(480);
	ChooseScaleFactor();

	server=settings.value(
		"remote/server",
		QString::fromStdString(server)
	).toString().toStdString();
}

void WebRTCScreen::setStatus(Status s){
	status=s;
	action_connect->setDisabled(true);
	action_disconnect->setDisabled(true);
	if(s==Connected)action_disconnect->setDisabled(false);
	if(s==Disconnected)action_connect->setDisabled(false);
	auto str=QMetaEnum::fromType<WebRTCScreen::Status>().valueToKey(s);
	log_info("status changed to {}",str);
	status_status->setText(str);
}

WebRTCScreen::~WebRTCScreen(){
	Disconnect();
}

void WebRTCScreen::Retranslate(){
	setWindowTitle("WebRTC KVM Client");
        menu_connection->setTitle("Connection");
        menu_scale->setTitle("Scale Factor");
        action_connect->setText("Connect");
        action_disconnect->setText("Disconnect");
        action_logout->setText("Logout");
}
