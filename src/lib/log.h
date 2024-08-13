/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef LOG_H
#define LOG_H
#include<format>
#include<source_location>
#include"defines.h"
class log{
	public:
		enum log_level{
			LOG_TRACE    = 0x1,
			LOG_VERBOSE  = 0x2,
			LOG_DEBUG    = 0x3,
			LOG_INFO     = 0x4,
			LOG_WARNING  = 0x5,
			LOG_ERROR    = 0x6,
			LOG_FATAL    = 0x7,
		};
		static std::string level2string(log_level level);
		static std::string relative_filename(std::source_location c);
		static void print(
			log_level level,
			const std::string&msg,
			std::source_location c=std::source_location::current()
		);

		static bool check_level(log_level level){return level>=current;}
		static log_level current;
};
#define log_trace(...) log::print(log::log_level::LOG_TRACE,   std::format(__VA_ARGS__))
#define log_verb(...)  log::print(log::log_level::LOG_VERBOSE, std::format(__VA_ARGS__))
#define log_dbg(...)   log::print(log::log_level::LOG_DEBUG,   std::format(__VA_ARGS__))
#define log_info(...)  log::print(log::log_level::LOG_INFO,    std::format(__VA_ARGS__))
#define log_warn(...)  log::print(log::log_level::LOG_WARNING, std::format(__VA_ARGS__))
#define log_err(...)   log::print(log::log_level::LOG_ERROR,   std::format(__VA_ARGS__))
#define log_fatal(...) log::print(log::log_level::LOG_FATAL,   std::format(__VA_ARGS__))
#endif
