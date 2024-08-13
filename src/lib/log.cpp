/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"log.h"

log::log_level log::current=log::log_level::LOG_DEBUG;

std::string log::level2string(log::log_level level){
	switch(level){
		case LOG_TRACE:return "TRACE";
		case LOG_VERBOSE:return "VERBOSE";
		case LOG_DEBUG:return "DEBUG";
		case LOG_INFO:return "INFO";
		case LOG_WARNING:return "WARNING";
		case LOG_ERROR:return "ERROR";
		case LOG_FATAL:return "FATAL";
		default:return "";
	}
}

std::string log::relative_filename(std::source_location c){
	std::string filename=c.file_name();
	auto i=filename.find("/src/");
	if(i!=std::string::npos)filename=filename.substr(i+5);
	return filename;
}

void log::print(log::log_level level,const std::string&msg,std::source_location c){
	if(likely(!check_level(level)))return;
	auto filename=relative_filename(c);
	auto dest=std::format(
		"{}({}:{}): {}",
		level2string(level),
		filename,c.line(),msg
	);
	puts(dest.c_str());
}
