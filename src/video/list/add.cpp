/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"
#include"lib/exception.h"

void StreamList::Add(Stream*stream){
	if(unlikely(!stream))throw InvalidArgument("stream is null");
	auto id=stream->GetID();
	auto uuid=stream->GetUUID();
	if(FindByUUID(uuid)||FindByID(id))
		throw InvalidArgument("stream already exists");
	streams_by_uuid[uuid]=stream;
	streams_by_id[id]=stream;
}

void StreamList::Remove(Stream*stream){
	auto id=stream->GetID();
	auto uuid=stream->GetUUID();
	auto idx_id=streams_by_id.find(id);
	auto idx_uuid=streams_by_uuid.find(uuid);
	if(idx_id==streams_by_id.end()||idx_uuid==streams_by_uuid.end())
		throw InvalidArgument("stream not found");
	if(idx_id->second!=stream||idx_uuid->second!=stream)
		throw InvalidArgument("stream mismatch");
	streams_by_id.erase(idx_id);
	streams_by_uuid.erase(idx_uuid);
}
