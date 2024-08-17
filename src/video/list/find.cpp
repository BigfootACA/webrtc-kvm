/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../stream.h"

Stream*StreamList::FindByUUID(const UUID&uuid)const{
	auto idx_uuid=streams_by_uuid.find(uuid);
	if(idx_uuid==streams_by_uuid.end())return nullptr;
	return idx_uuid->second;
}

Stream*StreamList::FindByID(const std::string&id)const{
	auto idx_id=streams_by_id.find(id);
	if(idx_id==streams_by_id.end())return nullptr;
	return idx_id->second;
}
