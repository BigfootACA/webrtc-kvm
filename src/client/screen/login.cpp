/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"screen.h"
#include"lib/log.h"
#include"lib/exception.h"
#include"../login/login.h"
#include<QSettings>

class LoginContext{
	public:
		LoginContext(
			WebRTCScreen*screen,
			std::function<void()>success,
			std::function<void(const std::string&)>failed
		);
		void AskLogin(const std::string&reason="");
		void RequestLogin(const QString&username,const QString&password);
		void onAuthLogin(const Json::Value&,QNetworkReply*);
		void onAuthInfo(const Json::Value&,QNetworkReply*);
		void TryAutoLogin();
		void Success();
		void Failed(const std::string&reason);
		WebRTCScreen*screen;
		std::function<void()>success;
		std::function<void(const std::string&)>failed;
};

void LoginContext::Success(){
	try{
		if(success)success();
		delete this;
	}catch(std::exception&exc){
		log_warn("error in login success callback: {}",exc.what());
		Failed(exc.what());
	}
}

void LoginContext::Failed(const std::string&reason){
	try{
		if(failed)failed(reason);
	}catch(std::exception&exc){
		log_warn("error in login failed callback: {}",exc.what());
	}
	delete this;
}

void LoginContext::AskLogin(const std::string&reason){
	LoginDialog ld(screen);
	if(auto v=screen->auth_info["username"])
		ld.edit_username->setText(QString::fromStdString(v.asString()));
	if(screen->auth_info["hide_username"].asBool())
		ld.edit_username->setDisabled(true);
	auto username=screen->settings.value("auth/username","").toString();
	auto password=screen->settings.value("auth/password","").toString();
	ld.check_save->setChecked(screen->settings.value("auth/save_password",false).toBool());
	ld.check_auto->setChecked(screen->settings.value("auth/auto_login",false).toBool());
	if(!username.isEmpty())ld.edit_username->setText(username);
	if(!password.isEmpty())ld.edit_password->setText(password);
	ld.show();
	if(ld.edit_username->text().isEmpty()&&ld.edit_username->isEnabled())
		ld.edit_username->setFocus();
	else ld.edit_password->setFocus();
	int r=ld.exec();
	if(r!=QDialog::DialogCode::Accepted){
		Failed("login canceled");
		return;
	}
	username=ld.edit_username->text();
	password=ld.edit_password->text();
	screen->settings.setValue("auth/save_password",ld.check_save->isChecked());
	screen->settings.setValue("auth/auto_login",ld.check_auto->isChecked());
	if(ld.check_save->isChecked()){	
		screen->settings.setValue("auth/username",username);
		screen->settings.setValue("auth/password",password);
	}
	RequestLogin(username,password);
}

void LoginContext::RequestLogin(const QString&username,const QString&password){
	Json::Value req;
	req["username"]=username.toStdString();
	req["password"]=password.toStdString();
	screen->HttpCall("/api/auth/login",req,[this](auto x,auto r){
		onAuthLogin(x,r);
	},[this](auto,auto,auto r){
		AskLogin(r);
	});
}

void LoginContext::onAuthLogin(const Json::Value&x,QNetworkReply*){
	screen->token=UUID(x["token"].asString());
	auto s=QString::fromStdString(screen->token.ToString());
	screen->settings.setValue("auth/token",s);
	log_info("login successful");
	Success();
}

void LoginContext::onAuthInfo(const Json::Value&x,QNetworkReply*){
	screen->auth_info=x;
	if(x["no_login"].asBool()){
		Success();
		return;
	}
	try{
		auto t=screen->settings.value("auth/token","").toString();
		if(t.isEmpty())throw InvalidArgument("no token");
		auto u=UUID(t.toStdString());
		if(u.IsNull())throw InvalidArgument("bad token");
		Json::Value req;
		req["token"]=u.ToString();
		screen->HttpCall("/api/auth/check",req,[this,u](auto,auto){
			log_info("use previous token");
			screen->token=u;
			Success();
		},[this](auto,auto,auto){
			screen->settings.remove("auth/token");
			TryAutoLogin();
		});
	}catch(...){
		TryAutoLogin();
	}
}

void LoginContext::TryAutoLogin(){
	try{
		if(!screen->settings.value("auth/auto_login",false).toBool())
			throw InvalidArgument("auto login disabled");
		auto username=screen->settings.value("auth/username","").toString();
		auto password=screen->settings.value("auth/password","").toString();
		if(username.isEmpty()||password.isEmpty())
			throw InvalidArgument("no username or password");
		RequestLogin(username,password);
	}catch(...){
		AskLogin();
	}
}

LoginContext::LoginContext(
	WebRTCScreen*screen,
	std::function<void()>success,
	std::function<void(const std::string&)>failed
):screen(screen),success(success),failed(failed){
	screen->HttpCall("/api/auth/info",{},[this](auto x,auto r){
		onAuthInfo(x,r);
	},[this](auto,auto,auto s){
		Failed(s);
	});
}

void WebRTCScreen::TryLogin(
	std::function<void()>success,
	std::function<void(const std::string&)>failed
){
	new LoginContext(this,success,failed);
}

void WebRTCScreen::Logout(){
	if(status!=Connected&&status!=Disconnected)return;
	log_info("logout");
	HttpCall("/api/auth/logout");
	settings.remove("auth/auto_login");
	settings.remove("auth/token");
	token.Clear();
	Disconnect();
}
