/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef FILES_H
#define FILES_H
#ifdef __cplusplus
#include<string>
#include<memory>
#include<vector>
#include<dirent.h>
#include<unistd.h>
#include<sys/types.h>
extern bool PathExists(const std::string&path);
extern std::string PathReadLink(const std::string&path);
extern void CreateFolder(const std::string&path,mode_t mode=0755,bool exists=false);
extern void RemoveFolder(const std::string&path,bool missing=false);
extern void CreateSymlink(const std::string&target,const std::string&link_name);
extern void RemoveFile(const std::string&path,bool missing=false);
extern std::string ReadFileString(const std::string&path,bool crlf=false);
extern int64_t ReadFileNumber(const std::string&path);
extern uint64_t ReadFileUnsigned(const std::string&path);
extern uint64_t ReadFileHex(const std::string&path);
extern size_t WriteFileString(const std::string&path,const std::string&value);
class FolderReader;

class FolderReaderItem{
	public:
		FolderReaderItem(const FolderReader*reader,const dirent*dt);
		[[nodiscard]] inline std::string GetName()const{return dt.d_name;}
		[[nodiscard]] inline unsigned int GetType()const{return dt.d_type;}
		[[nodiscard]] inline bool IsVirtual()const{return GetName()=="."||GetName()=="..";}
		[[nodiscard]] inline bool IsHidden()const{return dt.d_name[0]=='.';}
		[[nodiscard]] inline bool IsFIFOPipe()const{return dt.d_type==DT_FIFO;}
		[[nodiscard]] inline bool IsCharDevice()const{return dt.d_type==DT_CHR;}
		[[nodiscard]] inline bool IsFolder()const{return dt.d_type==DT_DIR;}
		[[nodiscard]] inline bool IsBlockDevice()const{return dt.d_type==DT_BLK;}
		[[nodiscard]] inline bool IsFile()const{return dt.d_type==DT_REG;}
		[[nodiscard]] inline bool IsSymlink()const{return dt.d_type==DT_LNK;}
		[[nodiscard]] inline bool IsSocket()const{return dt.d_type==DT_SOCK;}
	private:
		const FolderReader*reader;
		dirent dt{};
};

class FolderReader{
	public:
		explicit FolderReader(const std::string&path);
		~FolderReader();
		std::vector<std::shared_ptr<FolderReaderItem>>ReadAll();
		std::shared_ptr<FolderReaderItem>ReadItem();
	private:
		DIR*d;	
};
#endif
#endif
