/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef PLUGIN_H
#define PLUGIN_H
#include<map>
#include<string>
#include<memory>
class Plugin{
	public:
		~Plugin(){dlclose(handler);}
		inline void*ToHandler(){return handler;}
		inline explicit operator void*(){return ToHandler();}
		void*LookupRAW(const std::string&sym);
		template<typename T>T Lookup(const std::string&sym){return (T)LookupRAW(sym);}
		static std::shared_ptr<Plugin>Open(const std::string&plugin);
		static inline void*OpenSymbolRAW(
			const std::string&plugin,
			const std::string&symbol
		){
			return Open(plugin)->LookupRAW(symbol);
		}
		template<typename T>static inline T OpenSymbol(const std::string&plugin,const std::string&symbol){
			return (T)OpenSymbolRAW(plugin,symbol);
		}
	private:
		explicit Plugin(const std::string&name);
		std::string name;
		void*handler;
		static std::map<std::string,std::shared_ptr<Plugin>>loaded;
};
#endif
