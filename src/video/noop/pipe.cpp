/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"noop.h"

void NoopPipe::OnInitialize(){

}

void NoopPipe::OnDeinitialize(){

}

void NoopPipe::OnStartStream(){

}

void NoopPipe::OnStopStream(){

}

void NoopPipe::OnLoadDeviceConfig(YAML::Node&cfg){

}

void NoopPipe::OnProcessInput(StreamBuffer*buffer){
	SendToNext(buffer);
}

void NoopPipe::OnBindLink(std::shared_ptr<StreamLink>link,StreamLinkDirection dir){
	if(fourcc_in==0&&fourcc_out!=0)fourcc_in=fourcc_out;
	if(fourcc_out==0&&fourcc_in!=0)fourcc_out=fourcc_in;
}
