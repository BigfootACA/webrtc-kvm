/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"internal.h"
#include"shared.h"

void GadgetBase::Remove(){
	log_verb("{} remove {}",GetType(),GetName());
	RemoveFolder(GetPath());
}

void GadgetBase::Create(){
	auto path=GetPath();
	if(PathExists(path))return;
	log_verb("{} create {}",GetType(),GetName());
	CreateFolder(path);
}

void GadgetBase::Recreate(){
	log_verb("{} recreate {}",GetType(),GetName());
	Remove();
	Create();
}
