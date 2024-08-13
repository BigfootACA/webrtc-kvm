/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<format>
#include<fstream>
#include<json/json.h>
#include"configfile.h"
#include"exception.h"
#include"strings.h"

ConfigFile*ConfigFile::cfg=nullptr;
static const ConfigItem item_null;

static void LoadJSONRecursive(Json::Value&in,ConfigItem&out){
	switch(in.type()){
		case Json::nullValue:out=ConfigItem();break;
		case Json::intValue:out=ConfigItem((int64_t)in.asInt64());break;
		case Json::uintValue:out=ConfigItem((int64_t)in.asUInt64());break;
		case Json::realValue:out=ConfigItem((long double)in.asDouble());break;
		case Json::stringValue:out=ConfigItem((std::string)in.asString());break;
		case Json::booleanValue:out=ConfigItem((bool)in.asBool());break;
		case Json::arrayValue:
			out=ConfigItem(CONFIG_TYPE_ARRAY);
			for(auto v:in){
				ConfigItem item;
				LoadJSONRecursive(v,item);
				out.Add(item);
			}
		break;
		case Json::objectValue:
			out=ConfigItem(CONFIG_TYPE_OBJECT);
			for(auto v:in.getMemberNames()){
				ConfigItem item;
				LoadJSONRecursive(in[v],item);
				out[v]=item;
			}
		break;
		default:throw InvalidArgument("unknown type");
	}
}

ConfigItem ConfigItem::FromJSON(const std::string&content){
	Json::Reader reader;
	Json::Value val;
	ConfigItem root;
	if(!reader.parse(content,val))throw RuntimeError(
		"parse json failed: {}",
		reader.getFormattedErrorMessages()
	);
	LoadJSONRecursive(val,root);
	return root;
}

ConfigItem ConfigItem::FromJSON(std::istream&is){
	Json::Reader reader;
	Json::Value val;
	ConfigItem root;
	if(!reader.parse(is,val))throw RuntimeError(
		"parse json failed: {}",
		reader.getFormattedErrorMessages()
	);
	LoadJSONRecursive(val,root);
	return root;
}

void ConfigFile::ParseJSON(const std::string&file){
	Json::Reader reader;
	Json::Value val;
	std::ifstream in(file,std::ios::binary);
	if(!in.is_open())throw ErrnoException("open json {} failed",file);
	root=ConfigItem::FromJSON(in);
}

void ConfigFile::Parse(const std::string&file){
	std::string fmt=file;
	StringToLower(fmt);
	if(fmt.ends_with(".jsn")||fmt.ends_with(".json"))ParseJSON(file);
	else if(fmt.ends_with(".yml")||fmt.ends_with(".yaml"))ParseYAML(file);
	else throw InvalidArgument("unsupported config file type");
}

void ConfigFile::ParseYAML(const std::string&file){
	throw RuntimeError("yaml unimplemented");
}

size_t ConfigItem::GetLength()const{
	switch(type){
		case CONFIG_TYPE_STRING:return val_string.size();
		case CONFIG_TYPE_INTEGER:return sizeof(val_integer);
		case CONFIG_TYPE_FLOAT:return sizeof(val_float);
		case CONFIG_TYPE_BOOLEAN:return sizeof(bool);
		case CONFIG_TYPE_BYTES:return val_bytes->GetLength();
		case CONFIG_TYPE_OBJECT:return val_object.size();
		case CONFIG_TYPE_ARRAY:return val_array.size();
		default:return 0;
	}
}

std::string ConfigItem::ToFormattedString()const{
	std::string buf;
	bool first=true;
	switch(type){
		case CONFIG_TYPE_NULL:buf="null";
		case CONFIG_TYPE_STRING:
			buf+="\"";
			for(auto c:val_string){
				switch(c){
					case '\"':buf+="\\\"";continue;
					case '\n':buf+="\\n";continue;
					case '\r':buf+="\\r";continue;
					case '\t':buf+="\\t";continue;
					default:;
				}
				if(!isprint(c))buf+=std::format("\\x{0:2x}",c&0xFF);
				buf+=c;
			}
			buf+="\"";
		break;
		case CONFIG_TYPE_BYTES:
			buf+="\"";
			for(auto c:*val_bytes)
				buf+=std::format("\\x{0:2x}",c&0xFF);
			buf+="\"";
		break;
		case CONFIG_TYPE_OBJECT:
			buf+="{";
			for(auto&i:val_object){
				if(!first)buf+=",";
				first=false;
				buf+="\""+i.first+"\":";
				buf+=i.second.ToFormattedString();
			}
			buf+="}";
		break;
		case CONFIG_TYPE_ARRAY:
			buf+="[";
			for(auto&i:val_array){
				if(!first)buf+=",";
				first=false;
				buf+=i.ToFormattedString();
			}
			buf+="]";
		break;
		default:buf=ToString();
	}
	return buf;
}

std::string ConfigItem::ToString()const{
	switch(type){
		case CONFIG_TYPE_NULL:return "";
		case CONFIG_TYPE_STRING:return val_string;
		case CONFIG_TYPE_INTEGER:return std::to_string(val_integer);
		case CONFIG_TYPE_FLOAT:return std::to_string(val_float);
		case CONFIG_TYPE_BOOLEAN:return BOOL2STR(val_boolean);
		case CONFIG_TYPE_BYTES:return val_bytes->ToString();
		default:throw InvalidArgument("bad std::string");
	}
}

std::vector<std::string>ConfigItem::GetKeys()const{
	std::vector<std::string>ret;
	uint64_t num=0;
	switch(type){
		case CONFIG_TYPE_NULL:break;
		case CONFIG_TYPE_OBJECT:
			for(const auto&i:val_object)
				ret.push_back(i.first);
		break;
		case CONFIG_TYPE_ARRAY:
			for(num=0;num<val_array.size();num++)
				ret.push_back(std::to_string(num));
		break;
		default:throw InvalidArgument("bad keys");
	}
	return ret;
}

std::map<std::string,ConfigItem>ConfigItem::ToMap()const{
	std::map<std::string,ConfigItem>ret;
	uint64_t num=0;
	switch(type){
		case CONFIG_TYPE_NULL:break;
		case CONFIG_TYPE_OBJECT:ret=val_object;break;
		case CONFIG_TYPE_ARRAY:
			for(const auto& i:val_array)
				ret[std::to_string(num++)]=i;
		break;
		default:throw InvalidArgument("bad map");
	}
	return ret;
}

std::vector<ConfigItem>ConfigItem::ToVector()const{
	std::vector<ConfigItem>ret;
	switch(type){
		case CONFIG_TYPE_NULL:break;
		case CONFIG_TYPE_OBJECT:
			for(const auto&i:val_object)
				ret.push_back(i.second);
		break;
		case CONFIG_TYPE_ARRAY:return val_array;
		default:throw InvalidArgument("bad map");
	}
	return ret;
}

int64_t ConfigItem::ToInteger()const{
	switch(type){
		case CONFIG_TYPE_NULL:return 0;
		case CONFIG_TYPE_INTEGER:return val_integer;
		case CONFIG_TYPE_FLOAT:return (int64_t)val_float;
		case CONFIG_TYPE_BOOLEAN:return val_boolean?1:0;
		case CONFIG_TYPE_STRING:{
			bool fail=true;
			auto r=StringToNumber(val_string,(int64_t)0,&fail);
			if(!fail)return r;
		}//fallthrough
		default:throw InvalidArgument("bad integer");
	}
}

long double ConfigItem::ToFloat()const{
	switch(type){
		case CONFIG_TYPE_NULL:return 0;
		case CONFIG_TYPE_INTEGER:return val_integer;
		case CONFIG_TYPE_FLOAT:return val_float;
		case CONFIG_TYPE_BOOLEAN:return val_boolean?1:0;
		case CONFIG_TYPE_STRING:{
			bool fail=true;
			auto r=StringToNumber(val_string,(long double)0,&fail);
			if(!fail)return r;
		}//fallthrough
		default:throw InvalidArgument("bad float");
	}
}

bool ConfigItem::ToBoolean()const{
	switch(type){
		case CONFIG_TYPE_NULL:return false;
		case CONFIG_TYPE_INTEGER:return val_integer!=0;
		case CONFIG_TYPE_FLOAT:return val_float!=0;
		case CONFIG_TYPE_BOOLEAN:return val_boolean;
		case CONFIG_TYPE_STRING:{
			if(StringIsFalse(val_string))return false;
			if(StringIsTrue(val_string))return true;
		}//fallthrough
		default:throw InvalidArgument("bad boolean");
	}
}

bool ConfigItem::IsEmpty() const{
	switch(type){
		case CONFIG_TYPE_STRING:return val_string.empty();
		case CONFIG_TYPE_BYTES:return val_bytes->IsEmpty();
		case CONFIG_TYPE_OBJECT:return val_object.empty();
		case CONFIG_TYPE_ARRAY:return val_array.empty();
		case CONFIG_TYPE_INTEGER:
		case CONFIG_TYPE_FLOAT:
		case CONFIG_TYPE_BOOLEAN:
			return false;
		default:return true;
	}
}

ConfigItem&ConfigItem::Lookup(uint64_t idx){
	if(IsNull())type=CONFIG_TYPE_ARRAY;
	if(!IsArray())throw InvalidArgument("bad array");
	if(idx>=val_array.size())val_array.resize(idx+2);
	return val_array[idx];
}

ConfigItem&ConfigItem::Lookup(const std::string&key){
	if(IsNull())type=CONFIG_TYPE_OBJECT;
	if(!IsObject())throw InvalidArgument("bad object");
	if(!val_object.contains(key))
		val_object[key]=ConfigItem();
	return val_object[key];
}

const ConfigItem&ConfigItem::Lookup(uint64_t idx)const{
	if(IsNull())return item_null;
	if(!IsArray())throw InvalidArgument("bad array");
	if(idx>=val_array.size())return item_null;
	return val_array.at(idx);
}

const ConfigItem&ConfigItem::Lookup(const std::string&key)const{
	if(IsNull())return item_null;
	if(!IsObject())throw InvalidArgument("bad object");
	if(!val_object.contains(key))return item_null;
	return val_object.at(key);
}

template<typename T> int ValueCompare(const T lv,const T rv){
	if(lv>rv)return 1;
	if(lv<rv)return -1;
	return 0;
}

template<typename T> int VectorCompare(
	const std::vector<T>lv,
	const std::vector<T>rv
){
	size_t ll=lv.size(),rl=rv.size();
	if(ll!=rl)return ValueCompare(ll,rl);
	for(size_t i=0;i<ll;i++){
		auto r=ValueCompare(lv[i],rv[i]);
		if(r!=0)return r;
	}
	return 0;
}

template<typename Key,typename Value> int MapCompare(
	const std::map<Key,Value>lv,
	const std::map<Key,Value>rv
){
	int s;
	size_t ll=lv.size(),rl=rv.size();
	if(ll!=rl)return ValueCompare(ll,rl);
	for(
		auto l=lv.begin(),r=rv.begin();
		l!=lv.end()&&r!=rv.end();
		l++,r++
	){
		s=ValueCompare(l->first,r->first);
		if(s!=0)return s;
		s=ValueCompare(l->second,r->second);
		if(s!=0)return s;
	}
	return 0;
}

int ConfigItem::Compare(const ConfigItem&item)const{
	if(type!=item.type)
		return ValueCompare(type,item.type);
	switch(type){
		case CONFIG_TYPE_STRING:return ValueCompare(val_string,item.val_string);
		case CONFIG_TYPE_INTEGER:return ValueCompare(val_integer,item.val_integer);
		case CONFIG_TYPE_FLOAT:return ValueCompare(val_float,item.val_float);
		case CONFIG_TYPE_BOOLEAN:return ValueCompare(val_boolean,item.val_boolean);
		case CONFIG_TYPE_BYTES:return ValueCompare(val_integer,item.val_integer);
		case CONFIG_TYPE_OBJECT:return MapCompare(val_object,item.val_object);
		case CONFIG_TYPE_ARRAY:return VectorCompare(val_array,item.val_array);
		default:return 0;
        }
}

void ConfigItem::Add(const ConfigItem&item){
	if(!IsArray())throw InvalidArgument("bad array");
	val_array.push_back(item);
}

bool ConfigItem::Contains(uint64_t idx)const{
	if(!IsArray())throw InvalidArgument("bad array");
	return idx<val_array.size();
}

bool ConfigItem::Contains(const std::string&key)const{
	if(!IsObject())throw InvalidArgument("bad object");
	return val_object.contains(key);
}
