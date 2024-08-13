/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include<map>
#include<vector>
#include<memory>
#include"algo/blob.h"
enum ConfigValueType{
	CONFIG_TYPE_NONE    = 0,
	CONFIG_TYPE_NULL    = 1,
	CONFIG_TYPE_STRING  = 2,
	CONFIG_TYPE_INTEGER = 3,
	CONFIG_TYPE_FLOAT   = 4,
	CONFIG_TYPE_BOOLEAN = 5,
	CONFIG_TYPE_BYTES   = 6,
	CONFIG_TYPE_OBJECT  = 7,
	CONFIG_TYPE_ARRAY   = 8,
};
class ConfigItem{
	public:
		size_t GetLength(void)const;
		std::string ToFormattedString(void)const;
		std::string ToString(void)const;
		int64_t ToInteger(void)const;
		long double ToFloat(void)const;
		bool ToBoolean(void)const;
		std::vector<ConfigItem>ToVector(void)const;
		std::map<std::string,ConfigItem>ToMap(void)const;
		std::vector<std::string>GetKeys(void)const;
		void Add(const ConfigItem&item);
		bool IsEmpty(void)const;
		bool Contains(uint64_t idx)const;
		bool Contains(const std::string&key)const;
		ConfigItem&Lookup(uint64_t idx);
		ConfigItem&Lookup(const std::string&key);
		const ConfigItem&Lookup(uint64_t idx)const;
		const ConfigItem&Lookup(const std::string&key)const;
		int Compare(const ConfigItem&item)const;
		inline bool Equals(const ConfigItem&item)const{return Compare(item)==0;}
		inline ConfigItem(ConfigValueType type):type(type){}
		inline ConfigItem(void):type(CONFIG_TYPE_NULL){}
		inline ConfigItem(bool val):type(CONFIG_TYPE_BOOLEAN),val_boolean(val){}
		inline ConfigItem(const std::string&val):type(CONFIG_TYPE_STRING),val_string(val){}
		inline ConfigItem(int64_t val):type(CONFIG_TYPE_INTEGER),val_integer(val){}
		inline ConfigItem(long double val):type(CONFIG_TYPE_FLOAT),val_float(val){}
		inline ConfigItem(std::shared_ptr<Blob>val):type(CONFIG_TYPE_BYTES),val_bytes(val){}
		inline ConfigItem(const std::map<std::string,ConfigItem>&val):type(CONFIG_TYPE_OBJECT),val_object(val){}
		inline ConfigItem(const std::vector<ConfigItem> val):type(CONFIG_TYPE_ARRAY),val_array(val){}
		inline bool HaveValue(void)const{return !IsEmpty();}
		inline bool IsType(ConfigValueType t){return type==t;}
		inline ConfigValueType GetType(void)const{return type;}
		inline ConfigItem&operator[](uint64_t idx){return Lookup(idx);}
		inline ConfigItem&operator[](const std::string&key){return Lookup(key);}
		inline ConfigItem&operator[](const char*key){return Lookup(std::string(key));}
		inline const ConfigItem&operator[](uint64_t idx)const{return Lookup(idx);}
		inline const ConfigItem&operator[](const std::string&key)const{return Lookup(key);}
		inline const ConfigItem&operator[](const char*key)const{return Lookup(std::string(key));}
		inline bool IsNull(void)const{return GetType()==CONFIG_TYPE_NULL;}
		inline bool IsString(void)const{return GetType()==CONFIG_TYPE_STRING;}
		inline bool IsInteger(void)const{return GetType()==CONFIG_TYPE_INTEGER;}
		inline bool IsFloat(void)const{return GetType()==CONFIG_TYPE_FLOAT;}
		inline bool IsBoolean(void)const{return GetType()==CONFIG_TYPE_BOOLEAN;}
		inline bool IsBytes(void)const{return GetType()==CONFIG_TYPE_BYTES;}
		inline bool IsObject(void)const{return GetType()==CONFIG_TYPE_OBJECT;}
		inline bool IsArray(void)const{return GetType()==CONFIG_TYPE_ARRAY;}
		inline operator std::string(void)const{return ToString();}
		inline operator int64_t(void)const{return ToInteger();}
		inline operator uint64_t(void)const{return ToInteger();}
		inline operator int32_t(void)const{return ToInteger();}
		inline operator uint32_t(void)const{return ToInteger();}
		inline operator int16_t(void)const{return ToInteger();}
		inline operator uint16_t(void)const{return ToInteger();}
		inline operator int8_t(void)const{return ToInteger();}
		inline operator uint8_t(void)const{return ToInteger();}
		inline operator long double(void)const{return ToFloat();}
		inline operator bool(void)const{return ToBoolean();}
		inline bool operator==(const ConfigItem&item)const{return Equals(item);}
		inline bool operator!=(const ConfigItem&item)const{return !Equals(item);}
		inline bool operator>(const ConfigItem&item)const{return Compare(item)>0;}
		inline bool operator<(const ConfigItem&item)const{return Compare(item)<0;}
		inline bool operator>=(const ConfigItem&item)const{return Compare(item)>=0;}
		inline bool operator<=(const ConfigItem&item)const{return Compare(item)<=0;}
		static ConfigItem FromJSON(const std::string&content);
		static ConfigItem FromJSON(std::istream&is);
	private:
		ConfigValueType type;
		std::string val_string;
		bool val_boolean;
		int64_t val_integer;
		long double val_float;
		std::shared_ptr<Blob>val_bytes;
		std::map<std::string,ConfigItem>val_object;
		std::vector<ConfigItem>val_array;
};
class ConfigFile{
	public:
		void Parse(const std::string&file);
		void ParseJSON(const std::string&file);
		void ParseYAML(const std::string&file);
		ConfigItem root;
		static inline ConfigFile*GetCurrentConfig(void){
			if(!cfg)cfg=new ConfigFile();
			return cfg;
		}
	private:
		inline ConfigFile(){};
		static ConfigFile*cfg;
};

#endif
