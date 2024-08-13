/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<cstring>
#include"files.h"
#include"exception.h"

FolderReaderItem::FolderReaderItem(const FolderReader*reader,const dirent*dt){
	this->reader=reader;
	memcpy(&this->dt,dt,sizeof(dirent));
}

FolderReader::FolderReader(const std::string&path){
	if(!(d=opendir(path.c_str())))throw ErrnoException("opendir {} failed",path);
}

FolderReader::~FolderReader(){
	closedir(d);
}

std::shared_ptr<FolderReaderItem>FolderReader::ReadItem(){
	dirent*dt;
	if(!(dt=readdir(d)))return nullptr;
	return std::make_shared<FolderReaderItem>(this,dt);
}

std::vector<std::shared_ptr<FolderReaderItem>>FolderReader::ReadAll(){
	std::shared_ptr<FolderReaderItem>item;
	std::vector<std::shared_ptr<FolderReaderItem>>items;
	while((item=ReadItem()))items.push_back(item);
	return items;
}
