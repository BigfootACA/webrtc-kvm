/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"../http.h"
#include"abstract/resource_release.h"
#include<security/pam_appl.h>

static std::string PAMErrorToString(int ret){
	switch(ret){
		case PAM_SUCCESS:return "Successful function return";
		case PAM_OPEN_ERR:return "dlopen failure when dynamically loading a service module";
		case PAM_SYMBOL_ERR:return "Symbol not found";
		case PAM_SERVICE_ERR:return "Error in service module";
		case PAM_SYSTEM_ERR:return "System error";
		case PAM_BUF_ERR:return "Memory buffer error";
		case PAM_PERM_DENIED:return "Permission denied";
		case PAM_AUTH_ERR:return "Authentication failure";
		case PAM_CRED_INSUFFICIENT:return "Can not access authentication data due to insufficient credentials";
		case PAM_AUTHINFO_UNAVAIL:return "Underlying authentication service an not retrieve authentication information ";
		case PAM_USER_UNKNOWN:return "User not known to the underlying authentication module";
		case PAM_MAXTRIES:return "Too many tries";
		case PAM_NEW_AUTHTOK_REQD:return "New authentication token required";
		case PAM_ACCT_EXPIRED:return "User account has expired";
		case PAM_SESSION_ERR:return "Can not make/remove an entry for the specified session";
		case PAM_CRED_UNAVAIL:return "Underlying authentication service can not retrieve user credentials unavailable";
		case PAM_CRED_EXPIRED:return "User credentials expired";
		case PAM_CRED_ERR:return "Failure setting user credentials";
		case PAM_NO_MODULE_DATA:return "No module specific data is present";
		case PAM_CONV_ERR:return "Conversation error";
		case PAM_AUTHTOK_ERR:return "Authentication token manipulation error";
		case PAM_AUTHTOK_RECOVERY_ERR:return "Authentication information cannot be recovered";
		case PAM_AUTHTOK_LOCK_BUSY:return "Authentication token lock busy";
		case PAM_AUTHTOK_DISABLE_AGING:return "Authentication token aging disabled";
		case PAM_TRY_AGAIN:return "Preliminary check by password service";
		case PAM_IGNORE:return "Ignore underlying account module";
		case PAM_ABORT:return "Critical error (?module fail now request)";
		case PAM_AUTHTOK_EXPIRED:return "user's authentication token has expired";
		case PAM_MODULE_UNKNOWN:return "module is not known";
		case PAM_BAD_ITEM:return "Bad item passed to";
		case PAM_CONV_AGAIN:return "conversation again";
		case PAM_INCOMPLETE:return "Auth incomplete";
		default:return "Unknown error";
	}
}

void HttpAuthorization::AuthPAM(const AuthRequest&req){
	int ret;
	pam_handle_t*pamh=nullptr;
	auto&pam=ctx->http.auth.pam;
	auto&al=pam.allowed;
	if(!al.empty()&&std::find(al.begin(),al.end(),req.username)==al.end())
		throw HttpError(MHD_HTTP_UNAUTHORIZED,"username unallowed");
	if(pam.service.empty())throw HttpError(MHD_HTTP_INTERNAL_SERVER_ERROR,"no pam service set");
	auto conversation=[](auto,auto,pam_response**resp,void*ptr)->int {
		if(resp)*resp=(pam_response*)ptr;
		return PAM_SUCCESS;
	};
	auto res=(pam_response*)malloc(sizeof(pam_response));
	if(!res)throw ErrnoException("failed to allocate pam_response");
	res->resp=strdup(req.password.c_str()),res->resp_retcode=0;
	PointerRelease res_rel(res);
	PointerRelease resp_rel(res->resp);
	pam_conv conv={.conv=conversation,.appdata_ptr=res};
	try{
		if((ret=pam_start(pam.service.c_str(),req.username.c_str(),&conv,&pamh))!=PAM_SUCCESS)
			throw RuntimeError("pam_start failed: {}",PAMErrorToString(ret));
	}catch(Exceptions::RuntimeErrorImpl&exc){
		res_rel.Take();
		resp_rel.Take();
		log_err("failed to initialize pam: {}",exc.what());
		throw HttpError(MHD_HTTP_INTERNAL_SERVER_ERROR,"initialize pam failed");
	}
	ResourceRelease<pam_handle_t*>pamh_rel(pamh,[ret](auto i){
		if(i)try{pam_end(i,ret);}catch(...){}
	});
	res_rel.Take();
	resp_rel.Take();
	try{
		if((ret=pam_authenticate(pamh,0))!=PAM_SUCCESS)
			throw RuntimeError("pam_authenticate failed: {}",PAMErrorToString(ret));
		if((ret=pam_acct_mgmt(pamh,0))!=PAM_SUCCESS)
			throw RuntimeError("pam_acct_mgmt failed: {}",PAMErrorToString(ret));
	}catch(std::exception&exc){
		log_err("pam authenticate failed: {}",exc.what());
		throw HttpError(MHD_HTTP_UNAUTHORIZED,"pam authenticate failed");
	}
}
