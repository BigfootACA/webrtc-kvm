/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"abstract/async-runner.h"

void Stream::EnablePoll(int events){
	if(!poll_id.IsNull())return;
	AsyncRunner::DefaultRunLater([this,events](auto){DirectEnablePoll(events);});
}

void Stream::DisablePoll(){
	if(poll_id.IsNull())return;
	AsyncRunner::DefaultRunLater([this](auto){DirectDisablePoll();});
}

void Stream::DirectEnablePoll(int events){
	if(!poll_id.IsNull())return;
	poll_id=ctx->loop->AddHandler(GetFD(),events,[this](auto e){ProcessEvent(e);});
}

void Stream::DirectDisablePoll(){
	if(poll_id.IsNull())return;
	ctx->loop->RemoveHandler(poll_id);
}
