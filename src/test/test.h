/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef TEST_H
#define TEST_H
#include<string>
#include"defines.h"
#include"lib/exception.h"

class UnitTest{
	public:
		void RegisterSelf();
		virtual std::string GetName()=0;
		virtual void RunTest()=0;
};

#define DECL_TEST(type)\
	class UnitTest##type:public UnitTest{\
		public:\
			inline UnitTest##type(){RegisterSelf();}\
			std::string GetName()final{return __FILE__"@"#type;}\
			void RunTest()final;\
	};\
	cdecl_attr_used UnitTest##type UnitTestInstance##type;\
	void UnitTest##type::RunTest()
#define ASSERT_TEST(expr) do{\
	std::string v_expr=#expr;\
	bool v_ret;\
	try{v_ret=(expr);}catch(std::exception&exc){\
		throw RuntimeError("unexception exception thrown in {}: {}",v_expr,exc.what());\
	}\
	if(!v_ret)throw RuntimeError("test assert failed: {}",v_expr);\
}while(0)
#define WANT_EXCEPTION(expr,e) do{\
	std::string v_expr=#expr;\
	try{\
		expr;\
		throw RuntimeError("required exception {} in {} is not thrown",#e,v_expr);\
	}catch(e&exc){}\
	catch(std::exception&exc){\
		throw RuntimeError("unexception exception thrown in {}: {}",v_expr,exc.what());\
	}\
}while(0)

#endif
