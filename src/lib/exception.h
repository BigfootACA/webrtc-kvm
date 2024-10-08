/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H
#include<string>
#include<format>
#include<cstring>
#include<source_location>
namespace Exceptions{
	class RuntimeErrorImpl:public std::exception{
		public:
			inline RuntimeErrorImpl()=default;
			explicit RuntimeErrorImpl(
				const std::string&msg,
				std::source_location c=std::source_location::current()
			);
			~RuntimeErrorImpl()override=default;
			[[nodiscard]] inline const char*what()const noexcept override{return msg.c_str();}
			inline std::string GetMessage(){return msg;}
			inline std::string GetOriginal(){return original;}
			inline std::source_location GetLocation(){return location;}
		protected:
			inline void SetMessage(const std::string&m){msg=m;}
			std::source_location location;
			std::string original;
			std::string msg;
	};
	class InvalidArgumentImpl:public RuntimeErrorImpl{
		public:
			inline InvalidArgumentImpl()=default;
			explicit InvalidArgumentImpl(
				const std::string&msg,
				std::source_location c=std::source_location::current()
			);
			virtual ~InvalidArgumentImpl()=default;
	};
	class ErrnoExceptionImpl:public RuntimeErrorImpl{
		public:
			inline ErrnoExceptionImpl()=default;
			explicit ErrnoExceptionImpl(
				int err,
				const std::string&msg,
				std::source_location c=std::source_location::current()
			);
			virtual ~ErrnoExceptionImpl()=default;
			int err=0;
			static bool IsErrno(std::exception&exc,int err);
	};
}
#ifndef BARE_EXCEPTIONS
#define RuntimeError(...) Exceptions::RuntimeErrorImpl(std::format(__VA_ARGS__))
#define InvalidArgument(...) Exceptions::InvalidArgumentImpl(std::format(__VA_ARGS__))
#define ErrnoExceptionWith(err,...) Exceptions::ErrnoExceptionImpl(err,std::format(__VA_ARGS__))
#define ErrnoException(...) Exceptions::ErrnoExceptionImpl(errno,std::format(__VA_ARGS__))
#endif
#endif
