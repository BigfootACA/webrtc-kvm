/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef STRINGS_H
#define STRINGS_H
#include<string>
#include<vector>
#include<fcntl.h>
#include<cstdint>
#include<cstdarg>
#include"defines.h"
extern void StringReplaceAll(std::string&str,const std::string&from,const std::string&to);
extern void StringToUpper(std::string&str);
extern void StringToLower(std::string&str);
extern int StringToNumber(const std::string&str,int def=0,bool*fail=nullptr);
extern long StringToNumber(const std::string&str,long def=0,bool*fail=nullptr);
extern long long StringToNumber(const std::string&str,long long def=0,bool*fail=nullptr);
extern unsigned int StringToNumber(const std::string&str,unsigned int def=0,bool*fail=nullptr);
extern unsigned long StringToNumber(const std::string&str,unsigned long def=0,bool*fail=nullptr);
extern unsigned long long StringToNumber(const std::string&str,unsigned long long def=0,bool*fail=nullptr);
extern float StringToNumber(const std::string&str,float def=0,bool*fail=nullptr);
extern double StringToNumber(const std::string&str,double def=0,bool*fail=nullptr);
extern long double StringToNumber(const std::string&str,long double def=0,bool*fail=nullptr);
extern bool StringIsTrue(std::string str);
extern bool StringIsFalse(std::string str);
extern char dec2hex(int dec,bool upper);
extern int hex2dec(char hex);
extern std::string ToHexString(uint64_t num,size_t len=0,bool prefix=true,bool upper=false);
extern bool CheckValid(const char*val,const char*valids);
header_func bool CheckValid(const std::string&val,const char*valids){return CheckValid(val.c_str(),valids);}
extern std::string GetFileProperty(const std::string&content,const std::string&key);
extern std::string DeviceGetUeventPath(mode_t type,dev_t dev);
extern std::string DeviceReadUevent(mode_t type,dev_t dev);
extern std::string DeviceGetUeventProperty(mode_t type,dev_t dev,const std::string&key);
extern std::string PathFromDevice(mode_t type,dev_t dev);
extern dev_t ParseDeviceMajorMinor(const std::string&mm);
extern dev_t GetDeviceMajorMinor(const std::string&path);
extern dev_t DeviceFromFD(int fd,mode_t type);
extern dev_t DeviceFromPath(const std::string&path,mode_t type);
extern std::string GetDeviceSubsystem(dev_t dev);
extern std::vector<dev_t>DevicesFromDevicePath(const std::string&path,const std::string&subsys);
extern std::vector<dev_t>DevicesFromFolder(const std::string&path);
extern std::vector<dev_t>DevicesFromSubsystem(const std::string&subsys);
extern std::vector<dev_t>DevicesFromBus(const std::string&bus);
extern std::string DeviceToString(dev_t d);
extern int DeviceOpen(mode_t type,dev_t dev,int flags=O_RDWR|O_CLOEXEC);
extern std::string NonEmpty(const std::string&str,const std::string&empty="(none)");
extern std::string LinuxVersionToString(uint32_t ver);
extern std::string CharPointerToString(const char*pointer,size_t size);
#define CharArrayToString(_array) CharPointerToString(_array,sizeof(_array))
#endif
