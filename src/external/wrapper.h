/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef WRAPPER_H
#define WRAPPER_H
#include<map>
#include<string>
#include<dlfcn.h>
#include"defines.h"
#include"lib/exception.h"
#define FUNC_DECL_ARGS0(...)
#define FUNC_DECL_ARGS1(a0,a1,...)a0 a1
#define FUNC_DECL_ARGS2(a0,a1,a2,a3,...)a0 a1,a2 a3
#define FUNC_DECL_ARGS3(a0,a1,a2,a3,a4,a5,...)a0 a1,a2 a3,a4 a5
#define FUNC_DECL_ARGS4(a0,a1,a2,a3,a4,a5,a6,a7,...)a0 a1,a2 a3,a4 a5,a6 a7
#define FUNC_DECL_ARGS5(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,...)a0 a1,a2 a3,a4 a5,a6 a7,a8 a9
#define FUNC_DECL_ARGS6(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,...)a0 a1,a2 a3,a4 a5,a6 a7,a8 a9,a10 a11
#define FUNC_DECL_ARGS7(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,...)a0 a1,a2 a3,a4 a5,a6 a7,a8 a9,a10 a11,a12 a13
#define FUNC_DECL_ARGS8(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,...)a0 a1,a2 a3,a4 a5,a6 a7,a8 a9,a10 a11,a12 a13,a14 a15
#define FUNC_DECL_ARGS9(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,...)a0 a1,a2 a3,a4 a5,a6 a7,a8 a9,a10 a11,a12 a13,a14 a15,a16 a17
#define FUNC_DECL_ARGS910(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,...)a0 a1,a2 a3,a4 a5,a6 a7,a8 a9,a10 a11,a12 a13,a14 a15,a16 a17,a18 a19
#define FUNC_DECL_ARGS_(cnt,...)FUNC_DECL_ARGS##cnt(__VA_ARGS__)
#define FUNC_DECL_ARGS(cnt,...)FUNC_DECL_ARGS_(cnt,__VA_ARGS__)
#define FUNC_CALL_ARGS0(...)
#define FUNC_CALL_ARGS1(a0,a1,...)a1
#define FUNC_CALL_ARGS2(a0,a1,a2,a3,...)a1,a3
#define FUNC_CALL_ARGS3(a0,a1,a2,a3,a4,a5,...)a1,a3,a5
#define FUNC_CALL_ARGS4(a0,a1,a2,a3,a4,a5,a6,a7,...)a1,a3,a5,a7
#define FUNC_CALL_ARGS5(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,...)a1,a3,a5,a7,a9
#define FUNC_CALL_ARGS6(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,...)a1,a3,a5,a7,a9,a11
#define FUNC_CALL_ARGS7(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,...)a1,a3,a5,a7,a9,a11,a13
#define FUNC_CALL_ARGS8(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,...)a1,a3,a5,a7,a9,a11,a13,a15
#define FUNC_CALL_ARGS9(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,...)a1,a3,a5,a7,a9,a11,a13,a15,a17
#define FUNC_CALL_ARGS910(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,...)a1,a3,a5,a7,a9,a11,a13,a15,a17,a19
#define FUNC_CALL_ARGS_(cnt,...)FUNC_CALL_ARGS##cnt(__VA_ARGS__)
#define FUNC_CALL_ARGS(cnt,...)FUNC_CALL_ARGS_(cnt,__VA_ARGS__)
#define ARGS_CNT_E(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,cnt,...)cnt
#define ARGS_CNT(...) ARGS_CNT_E(__VA_ARGS__,10,$bad,9,$bad,8,$bad,7,$bad,6,$bad,5,$bad,4,$bad,3,$bad,2,$bad,1,$bad,0)
#define EXT_WRAPPER(_ret,_name,...)\
	inline _ret _name(FUNC_DECL_ARGS(ARGS_CNT(__VA_ARGS__),__VA_ARGS__)){\
		return GetFunction<_ret(*)(FUNC_DECL_ARGS(ARGS_CNT(__VA_ARGS__),__VA_ARGS__))>(#_name)(\
			FUNC_CALL_ARGS(ARGS_CNT(__VA_ARGS__),__VA_ARGS__)\
		);\
	}
namespace Exceptions{
	class LibraryError:public Exceptions::RuntimeErrorImpl{
		public:
			inline LibraryError()=default;
			explicit LibraryError(
				const std::string&msg,
				std::source_location c=std::source_location::current()
			);
			~LibraryError()override=default;
	};
}
class LibraryLoader{
	public:
		virtual ~LibraryLoader();
		virtual std::string GetLibraryName()=0;
		void*GetLibrary();
		void*GetSymbol(const std::string&name);
		template<typename T>inline T GetFunction(const std::string&name){
			return (T)GetSymbol(name);
		}
	private:
		virtual void*LoadLibrary(const std::string&name);
		virtual void*LocateSymbol(const std::string&name);
		void*library=nullptr;
		std::map<std::string,void*>symbols;
};
#ifndef BARE_EXCEPTIONS
#define LibraryError(...) Exceptions::LibraryError(std::format(__VA_ARGS__))
#endif
#endif
