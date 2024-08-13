/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../test.h"
#include"lib/path.h"

DECL_TEST(PathResolve){
	ASSERT_TEST(PathResolve('/',"/mnt/test/")=="mnt/test/");
	ASSERT_TEST(PathResolve('/',"/mnt/bad/./.././test")=="mnt/test");
	ASSERT_TEST(PathResolve('/',"../../../../mnt")=="mnt");
	ASSERT_TEST(PathResolve('/',"")=="");
}

DECL_TEST(PathBaseName){
	ASSERT_TEST(PathBaseName("test")=="test");
	ASSERT_TEST(PathBaseName("/mnt/test")=="test");
	ASSERT_TEST(PathBaseName("/mnt/test/")=="test");
	ASSERT_TEST(PathBaseName("/mnt/test/folder")=="folder");
}
