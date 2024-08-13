/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"input/input.h"

class HttpInputGetKeyMapHandler:public HttpRestfulHandler{
	public:
		inline HttpInputGetKeyMapHandler(){RegisterSelf();}
		[[nodiscard]] inline std::string GetPath()const final{return "/api/input/get_key_map";}
		int HandleRestful(HttpRestfulRequestContext*ctx)final;
};

cdecl_attr_used HttpInputGetKeyMapHandler http_input_get_key_map_handler;

int HttpInputGetKeyMapHandler::HandleRestful(HttpRestfulRequestContext*ctx){
	ctx->response["map"]={Json::objectValue};
	for(uint32_t i=0;WebKeyMapData[i].name;i++)
		ctx->response["map"][WebKeyMapData[i].name]=WebKeyMapData[i].code;
	return MHD_HTTP_OK;
}
