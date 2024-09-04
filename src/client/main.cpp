/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<QApplication>
#include"lib/log.h"
#include"screen/screen.h"

int main(int argc,char**argv){
	QApplication app(argc,argv);
	QCoreApplication::setOrganizationName("ClassFun");
	QCoreApplication::setOrganizationDomain("classfun.cn");
	QCoreApplication::setApplicationName("WebRTC-KVM Client");
	log_info("WebRTC-KVM Client started");
	WebRTCScreen scr;
	scr.show();
	return QApplication::exec();
}
