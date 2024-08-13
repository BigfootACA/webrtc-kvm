/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef FULL_IO_H
#define FULL_IO_H
#include<unistd.h>

extern ssize_t full_write(int fd,const void*data,size_t len);
extern ssize_t full_read(int fd,void*data,size_t len);
extern ssize_t full_write_file(const char*path,const void*data,size_t len);
extern ssize_t full_read_file(const char*path,void*data,size_t len);
extern ssize_t full_read_file_alloc(const char*path,void**data);

#endif
