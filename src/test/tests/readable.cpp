/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../test.h"
#include"lib/readable.h"

DECL_TEST(FormatSize){
	ASSERT_TEST(FormatSize(0)=="0");
	ASSERT_TEST(FormatSize(1)=="1 B");
	ASSERT_TEST(FormatSize(869)=="869 B");
	ASSERT_TEST(FormatSize(1023)=="1023 B");
	ASSERT_TEST(FormatSize(1024)=="1 KiB");
	ASSERT_TEST(FormatSize(1536)=="1 KiB");
	ASSERT_TEST(FormatSize(2048)=="2 KiB");
	ASSERT_TEST(FormatSize(1048576)=="1 MiB");
}

DECL_TEST(FormatSizeFloat){
	ASSERT_TEST(FormatSizeFloat(0)=="0");
	ASSERT_TEST(FormatSizeFloat(1)=="1.0 B");
	ASSERT_TEST(FormatSizeFloat(5881476)   == "5.61 MiB");
	ASSERT_TEST(FormatSizeFloat(506165306) == "482.72 MiB");
	ASSERT_TEST(FormatSizeFloat(914294798) == "871.94 MiB");
	ASSERT_TEST(FormatSizeFloat(699009434) == "666.63 MiB");
	ASSERT_TEST(FormatSizeFloat(463170700) == "441.71 MiB");
	ASSERT_TEST(FormatSizeFloat(102464820) == "97.72 MiB");
	ASSERT_TEST(FormatSizeFloat(653740656) == "623.46 MiB");
	ASSERT_TEST(FormatSizeFloat(4409056)   == "4.2 MiB");
	ASSERT_TEST(FormatSizeFloat(155540408) == "148.33 MiB");
	ASSERT_TEST(FormatSizeFloat(171483147) == "163.54 MiB");
}
