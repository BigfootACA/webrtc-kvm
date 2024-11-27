/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<algorithm>
#include"compress.h"
#include"lib/exception.h"

std::vector<Compressor*>Compressor::compressor;

void Compressor::Register(Compressor*comp){
	auto type=comp->GetType();
	if(type.empty())throw InvalidArgument("bad compressor type");
	if(ByType(type))throw InvalidArgument("compressor already exists");
	compressor.push_back(comp);
}

void Compressor::Unregister(Compressor*comp){
	auto idx=std::find(compressor.begin(),compressor.end(),comp);
	if(idx==compressor.end())return;
	compressor.erase(idx);
}
