/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/exception.h"

void StreamLink::BindSource(Stream*stream){
	if(!stream)throw InvalidArgument("bad source to bind");
	if(source==stream)return;
	if(source)throw InvalidArgument("link already bound");
	if(stream->output)throw InvalidArgument(
		"source {} already bound to another link",
		stream->GetID()
	);
	source=stream;
	source->output=shared_from_this();
}

void StreamLink::BindSink(Stream*stream){
	if(!stream)throw InvalidArgument("bad sink to bind");
	if(sink==stream)return;
	if(sink)throw InvalidArgument("link already bound");
	if(stream->input)throw InvalidArgument(
		"sink {} already bound to another link",
		stream->GetID()
	);
	sink=stream;
	sink->input=shared_from_this();
}
