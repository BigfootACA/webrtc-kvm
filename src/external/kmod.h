/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef WRAPPER_KMOD_H
#define WRAPPER_KMOD_H
#include<fcntl.h>
#include<cstdarg>
#include<cstdint>
#include"wrapper.h"
struct kmod_ctx;
struct kmod_list;
struct kmod_module;
struct kmod_config_iter;
typedef void(kmod_log_fn)(void*,int,const char*,int,const char*,const char*,va_list);
typedef int(kmod_run_install)(kmod_module*,const char*,void*);
typedef void(kmod_print_action)(kmod_module*,bool,const char*);
#define kmod_list_foreach(kmod,list_entry,first_entry) for(list_entry=first_entry;list_entry!=NULL;list_entry=(kmod)->kmod_list_next(first_entry,list_entry))
#define kmod_list_foreach_reverse(kmod,list_entry,first_entry) for(list_entry=(kmod)->kmod_list_last(first_entry);list_entry!=NULL;list_entry=(kmod)->kmod_list_prev(first_entry,list_entry))
enum kmod_resources{
        KMOD_RESOURCES_OK = 0,
        KMOD_RESOURCES_MUST_RELOAD = 1,
        KMOD_RESOURCES_MUST_RECREATE = 2,
};
enum kmod_index {
        KMOD_INDEX_MODULES_DEP = 0,
        KMOD_INDEX_MODULES_ALIAS,
        KMOD_INDEX_MODULES_SYMBOL,
        KMOD_INDEX_MODULES_BUILTIN_ALIAS,
        KMOD_INDEX_MODULES_BUILTIN,
        _KMOD_INDEX_PAD = 1U << 31,
};
enum kmod_remove {
        KMOD_REMOVE_FORCE = O_TRUNC,
        KMOD_REMOVE_NOWAIT = O_NONBLOCK,
        KMOD_REMOVE_NOLOG = 1,
};
enum kmod_insert {
        KMOD_INSERT_FORCE_VERMAGIC = 0x1,
        KMOD_INSERT_FORCE_MODVERSION = 0x2,
};
enum kmod_probe {
        KMOD_PROBE_FORCE_VERMAGIC =             0x00001,
        KMOD_PROBE_FORCE_MODVERSION =           0x00002,
        KMOD_PROBE_IGNORE_COMMAND =             0x00004,
        KMOD_PROBE_IGNORE_LOADED =              0x00008,
        KMOD_PROBE_DRY_RUN =                    0x00010,
        KMOD_PROBE_FAIL_ON_LOADED =             0x00020,
        KMOD_PROBE_APPLY_BLACKLIST_ALL =        0x10000,
        KMOD_PROBE_APPLY_BLACKLIST =            0x20000,
        KMOD_PROBE_APPLY_BLACKLIST_ALIAS_ONLY = 0x40000,
};
enum kmod_filter{
        KMOD_FILTER_BLACKLIST = 0x00001,
        KMOD_FILTER_BUILTIN = 0x00002,
};
enum kmod_module_initstate{
        KMOD_MODULE_BUILTIN = 0,
        KMOD_MODULE_LIVE,
        KMOD_MODULE_COMING,
        KMOD_MODULE_GOING,
        _KMOD_MODULE_PAD = 1U << 31,
};

enum kmod_symbol_bind{
        KMOD_SYMBOL_NONE = '\0',
        KMOD_SYMBOL_LOCAL = 'L',
        KMOD_SYMBOL_GLOBAL = 'G',
        KMOD_SYMBOL_WEAK = 'W',
        KMOD_SYMBOL_UNDEF = 'U'
};

class KMOD:public LibraryLoader{
	public:inline std::string GetLibraryName()override{return "libkmod.so";}
EXT_WRAPPER(kmod_ctx*,kmod_new,const char*,dirname,const char*const*,config_paths)
EXT_WRAPPER(kmod_ctx*,kmod_ref,kmod_ctx*,ctx)
EXT_WRAPPER(kmod_ctx*,kmod_unref,kmod_ctx*,ctx)
EXT_WRAPPER(void,kmod_set_log_fn,kmod_ctx*,ctx,kmod_log_fn*,log_fn,const void*,data)
EXT_WRAPPER(int,kmod_get_log_priority,const kmod_ctx*,ctx)
EXT_WRAPPER(void,kmod_set_log_priority,kmod_ctx*,ctx,int,priority)
EXT_WRAPPER(void*,kmod_get_userdata,const kmod_ctx*,ctx)
EXT_WRAPPER(void,kmod_set_userdata,kmod_ctx*,ctx,const void*,userdata)
EXT_WRAPPER(const char*,kmod_get_dirname,const kmod_ctx*,ctx)
EXT_WRAPPER(int,kmod_load_resources,kmod_ctx*,ctx)
EXT_WRAPPER(void,kmod_unload_resources,kmod_ctx*,ctx)
EXT_WRAPPER(int,kmod_validate_resources,kmod_ctx*,ctx)
EXT_WRAPPER(int,kmod_dump_index,kmod_ctx*,ctx,kmod_index,type,int,fd)
EXT_WRAPPER(kmod_list*,kmod_list_next,const kmod_list*,list,const kmod_list*,curr)
EXT_WRAPPER(kmod_list*,kmod_list_prev,const kmod_list*,list,const kmod_list*,curr)
EXT_WRAPPER(kmod_list*,kmod_list_last,const kmod_list*,list)
EXT_WRAPPER(kmod_config_iter*,kmod_config_get_blacklists,const kmod_ctx*,ctx)
EXT_WRAPPER(kmod_config_iter*,kmod_config_get_install_commands,const kmod_ctx*,ctx)
EXT_WRAPPER(kmod_config_iter*,kmod_config_get_remove_commands,const kmod_ctx*,ctx)
EXT_WRAPPER(kmod_config_iter*,kmod_config_get_aliases,const kmod_ctx*,ctx)
EXT_WRAPPER(kmod_config_iter*,kmod_config_get_options,const kmod_ctx*,ctx)
EXT_WRAPPER(kmod_config_iter*,kmod_config_get_softdeps,const kmod_ctx*,ctx)
EXT_WRAPPER(const char*,kmod_config_iter_get_key,const kmod_config_iter*,iter)
EXT_WRAPPER(const char*,kmod_config_iter_get_value,const kmod_config_iter*,iter)
EXT_WRAPPER(bool,kmod_config_iter_next,kmod_config_iter*,iter)
EXT_WRAPPER(void,kmod_config_iter_free_iter,kmod_config_iter*,iter)
EXT_WRAPPER(int,kmod_module_new_from_name,kmod_ctx*,ctx,const char*,name,kmod_module**,mod)
EXT_WRAPPER(int,kmod_module_new_from_path,kmod_ctx*,ctx,const char*,path,kmod_module**,mod)
EXT_WRAPPER(int,kmod_module_new_from_lookup,kmod_ctx*,ctx,const char*,given_alias,kmod_list**,list)
EXT_WRAPPER(int,kmod_module_new_from_name_lookup,kmod_ctx*,ctx,const char*,modname,kmod_module**,mod)
EXT_WRAPPER(int,kmod_module_new_from_loaded,kmod_ctx*,ctx,kmod_list**,list)
EXT_WRAPPER(kmod_module*,kmod_module_ref,kmod_module*,mod)
EXT_WRAPPER(kmod_module*,kmod_module_unref,kmod_module*,mod)
EXT_WRAPPER(int,kmod_module_unref_list,kmod_list*,list)
EXT_WRAPPER(kmod_module*,kmod_module_get_module,const kmod_list*,entry)
EXT_WRAPPER(int,kmod_module_remove_module,kmod_module*,mod,unsigned int,flags)
EXT_WRAPPER(int,kmod_module_insert_module,kmod_module*,mod,unsigned int,flags,const char*,options)
EXT_WRAPPER(int,kmod_module_probe_insert_module,kmod_module*,mod,unsigned int,flags,const char*,extra_options,kmod_run_install*,run_install,const void*,data,kmod_print_action*,print_action)
EXT_WRAPPER(const char*,kmod_module_get_name,const kmod_module*,mod)
EXT_WRAPPER(const char*,kmod_module_get_path,const kmod_module*,mod)
EXT_WRAPPER(const char*,kmod_module_get_options,const kmod_module*,mod)
EXT_WRAPPER(const char*,kmod_module_get_install_commands,const kmod_module*,mod)
EXT_WRAPPER(const char*,kmod_module_get_remove_commands,const kmod_module*,mod)
EXT_WRAPPER(kmod_list*,kmod_module_get_dependencies,const kmod_module*,mod)
EXT_WRAPPER(int,kmod_module_get_softdeps,const kmod_module*,mod, kmod_list**,pre,kmod_list**,post)
EXT_WRAPPER(int,kmod_module_apply_filter,const kmod_ctx*,ctx,kmod_filter,filter_type,const kmod_list*,input,kmod_list**,output)
EXT_WRAPPER(const char*,kmod_module_initstate_str,kmod_module_initstate,state)
EXT_WRAPPER(int,kmod_module_get_initstate,const kmod_module*,mod)
EXT_WRAPPER(int,kmod_module_get_refcnt,const kmod_module*,mod)
EXT_WRAPPER(kmod_list*,kmod_module_get_holders,const kmod_module*,mod)
EXT_WRAPPER(kmod_list*,kmod_module_get_sections,const kmod_module*,mod)
EXT_WRAPPER(const char*,kmod_module_section_get_name,const kmod_list*,entry)
EXT_WRAPPER(unsigned long,kmod_module_section_get_address,const kmod_list*,entry)
EXT_WRAPPER(void,kmod_module_section_free_list,kmod_list*,list)
EXT_WRAPPER(long,kmod_module_get_size,const kmod_module*,mod)
EXT_WRAPPER(int,kmod_module_get_info,const kmod_module*,mod,kmod_list**,list)
EXT_WRAPPER(const char*,kmod_module_info_get_key,const kmod_list*,entry)
EXT_WRAPPER(const char*,kmod_module_info_get_value,const kmod_list*,entry)
EXT_WRAPPER(void,kmod_module_info_free_list,kmod_list*,list)
EXT_WRAPPER(int,kmod_module_get_versions,const kmod_module*,mod,kmod_list**,list)
EXT_WRAPPER(const char*,kmod_module_version_get_symbol,const kmod_list*,entry)
EXT_WRAPPER(uint64_t,kmod_module_version_get_crc,const kmod_list*,entry)
EXT_WRAPPER(void,kmod_module_versions_free_list,kmod_list*,list)
EXT_WRAPPER(int,kmod_module_get_symbols,const kmod_module*,mod,kmod_list**,list)
EXT_WRAPPER(const char*,kmod_module_symbol_get_symbol,const kmod_list*,entry)
EXT_WRAPPER(uint64_t,kmod_module_symbol_get_crc,const kmod_list*,entry)
EXT_WRAPPER(void,kmod_module_symbols_free_list,kmod_list*,list)
EXT_WRAPPER(int,kmod_module_get_dependency_symbols,const kmod_module*,mod,kmod_list**,list)
EXT_WRAPPER(const char*,kmod_module_dependency_symbol_get_symbol,const kmod_list*,entry)
EXT_WRAPPER(int,kmod_module_dependency_symbol_get_bind,const kmod_list*,entry)
EXT_WRAPPER(uint64_t,kmod_module_dependency_symbol_get_crc,const kmod_list*,entry)
EXT_WRAPPER(void,kmod_module_dependency_symbols_free_list,kmod_list*,list)
};
#endif
