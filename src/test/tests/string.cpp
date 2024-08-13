/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../test.h"
#include"lib/strings.h"
#include<climits>

DECL_TEST(StringToLower){
	ASSERT_TEST(({std::string str="abcd";StringToLower(str);str=="abcd";}));
	ASSERT_TEST(({std::string str="ABcd";StringToLower(str);str=="abcd";}));
	ASSERT_TEST(({std::string str="ABCD";StringToLower(str);str=="abcd";}));
	ASSERT_TEST(({std::string str="1234";StringToLower(str);str=="1234";}));
	ASSERT_TEST(({std::string str="!@#$";StringToLower(str);str=="!@#$";}));
}

DECL_TEST(StringToUpper){
	ASSERT_TEST(({std::string str="abcd";StringToUpper(str);str=="ABCD";}));
	ASSERT_TEST(({std::string str="ABcd";StringToUpper(str);str=="ABCD";}));
	ASSERT_TEST(({std::string str="ABCD";StringToUpper(str);str=="ABCD";}));
	ASSERT_TEST(({std::string str="1234";StringToUpper(str);str=="1234";}));
	ASSERT_TEST(({std::string str="!@#$";StringToUpper(str);str=="!@#$";}));
}

DECL_TEST(dec2hex){
	ASSERT_TEST(dec2hex(0,false)=='0');
	ASSERT_TEST(dec2hex(6,false)=='6');
	ASSERT_TEST(dec2hex(10,false)=='a');
	ASSERT_TEST(dec2hex(15,false)=='f');
	ASSERT_TEST(dec2hex(20,false)==0);
	ASSERT_TEST(dec2hex(0,true)=='0');
	ASSERT_TEST(dec2hex(6,true)=='6');
	ASSERT_TEST(dec2hex(10,true)=='A');
	ASSERT_TEST(dec2hex(15,true)=='F');
	ASSERT_TEST(dec2hex(20,true)==0);
}

DECL_TEST(hex2dec){
	ASSERT_TEST(hex2dec('0')==0);
	ASSERT_TEST(hex2dec('6')==6);
	ASSERT_TEST(hex2dec('a')==10);
	ASSERT_TEST(hex2dec('f')==15);
	ASSERT_TEST(hex2dec('z')==16);
	ASSERT_TEST(hex2dec('0')==0);
	ASSERT_TEST(hex2dec('6')==6);
	ASSERT_TEST(hex2dec('A')==10);
	ASSERT_TEST(hex2dec('F')==15);
	ASSERT_TEST(hex2dec('Z')==16);
}

DECL_TEST(StringToNumber){
	bool fail;
	ASSERT_TEST(StringToNumber("0",         (int)0,&fail)==0&&!fail);
	ASSERT_TEST(StringToNumber("-10",       (long)0,&fail)==-10&&!fail);
	ASSERT_TEST(StringToNumber("123456",    (unsigned long)0,&fail)==123456&&!fail);
	ASSERT_TEST(StringToNumber("-1",        (unsigned int)0,&fail)==UINT_MAX&&!fail);
	ASSERT_TEST(StringToNumber("1.1",       (float)0,&fail)==1.1f&&!fail);
	ASSERT_TEST(StringToNumber("1.1",       (double)0,&fail)==1.1&&!fail);
	ASSERT_TEST(StringToNumber("0x1000000", (unsigned long long)0,&fail)==0x1000000&&!fail);
	WANT_EXCEPTION(StringToNumber("abcd",   (unsigned long)0,&fail),std::invalid_argument);
}

DECL_TEST(ToHexString){
	ASSERT_TEST(ToHexString(0x1234a,0,true,false)=="0x1234a");
	ASSERT_TEST(ToHexString(0x7890a,8,false,false)=="0007890a");
	ASSERT_TEST(ToHexString(0x123456789abcdef,16,false,false)=="0123456789abcdef");
	ASSERT_TEST(ToHexString(0x123456789abcdef,17,true,true)=="0x00123456789ABCDEF");
}

DECL_TEST(CheckValid){
	ASSERT_TEST(CheckValid("9012","0123456789")==true);
	ASSERT_TEST(CheckValid("9a12","0123456789")==false);
	ASSERT_TEST(CheckValid("abcd","0123456789")==false);
	ASSERT_TEST(CheckValid("","0123456789")==true);
}
