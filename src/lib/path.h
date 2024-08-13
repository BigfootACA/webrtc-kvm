/*
 *
 * Copyright (C) 2023 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 * Path process functions
 *
 */

#ifndef PATH_H
#define PATH_H
#ifdef __cplusplus
extern "C"{
#endif
#include<stddef.h>

extern size_t path_resolve(char*buff,size_t size,char sep,const char*path);
extern size_t path_merge(char*buff,size_t size,char sep,const char*path1,const char*path2);
extern const char*path_basename(char*buff,size_t size,const char*path,size_t len);
extern const char*path_dirname(char*buff,size_t size,const char*path,size_t len);
extern const char*path_get_self(char*buff,size_t size);
extern const char*path_get_self_folder(char*buff,size_t size);
#ifdef __cplusplus
}
#include<string>
extern std::string PathResolve(char sep,const std::string&path);
extern std::string PathMerge(char sep,const std::string&path1,const std::string&path2);
extern std::string PathBaseName(const std::string&path);
extern std::string PathDirName(const std::string&path);
extern std::string PathGetSelf();
extern std::string PathGetSelfFolder();
#endif
#endif
