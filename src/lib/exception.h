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
	class RuntimeError:public std::exception{
		public:
			inline RuntimeError()=default;
			explicit RuntimeError(
				const std::string&msg,
				std::source_location c=std::source_location::current()
			);
			~RuntimeError()override=default;
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
	class InvalidArgument:public RuntimeError{
		public:
			inline InvalidArgument()=default;
			explicit InvalidArgument(
				const std::string&msg,
				std::source_location c=std::source_location::current()
			);
			virtual ~InvalidArgument()=default;
	};
	class ErrnoException:public RuntimeError{
		public:
			inline ErrnoException()=default;
			explicit ErrnoException(
				const std::string&msg,
				std::source_location c=std::source_location::current()
			);
			virtual ~ErrnoException()=default;
			int err=0;
	};
}
#ifndef BARE_EXCEPTIONS
#define RuntimeError(...) Exceptions::RuntimeError(std::format(__VA_ARGS__))
#define InvalidArgument(...) Exceptions::InvalidArgument(std::format(__VA_ARGS__))
#define ErrnoException(...) Exceptions::ErrnoException(std::format(__VA_ARGS__))
#endif
#endif
