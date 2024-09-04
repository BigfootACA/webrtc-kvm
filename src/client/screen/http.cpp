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

static void OnReply(
	const std::string&surl,
	QNetworkReply*reply,
	std::function<void(Json::Value&,QNetworkReply*)>success,
	std::function<void(Json::Value&,QNetworkReply*,const std::string&)>failed
){
	Json::Value resp;
	auto err=reply->error();
	auto reason=std::string(QMetaEnum::fromType<QNetworkReply::NetworkError>().valueToKey(err));
	try{
		Json::Reader rd;
		QString response=reply->readAll();
		if(response.isEmpty()&&err!=QNetworkReply::NoError)
			throw RuntimeError("request failed: {}",reason);
		if(!rd.parse(response.toStdString(),resp))throw RuntimeError(
			"parse json failed: {}",rd.getFormattedErrorMessages()
		);
		if(!resp["success"].asBool()){
			std::string msg;
			if(auto m=resp["message"])msg=m.asString();
			else if(auto c=resp["code"])msg=std::format("status {}",c.asInt());
			else msg="success is false";
			throw RuntimeError("response is not success: {}",msg);
		}
		log_info("request {} done",surl);
		if(success)success(resp,reply);
	}catch(std::exception&exc){
		log_warn("fetch {} response failed: {}",surl,exc.what());
		if(failed)failed(resp,reply,exc.what());
	}
	reply->deleteLater();
}

void WebRTCScreen::HttpCall(
	const std::string&path,
	Json::Value body,
	std::function<void(Json::Value&,QNetworkReply*)>success,
	std::function<void(Json::Value&,QNetworkReply*,const std::string&)>failed
){
	QNetworkReply*reply;
	Json::FastWriter wr;
	auto surl=std::format("{}{}",server,path);
	try{
		QUrl url(QString::fromStdString(surl));
		QNetworkRequest request(url);
		request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
		if(!token.IsNull()){
			auto v=std::format("Bearer {}",token.ToString());
			auto data=QString::fromStdString(v).toLocal8Bit();
			request.setRawHeader("Authorization",data);
		}
		auto req_body=wr.write(body);
		auto data=QByteArray::fromStdString(req_body);
		reply=net.post(request,data);
		QObject::connect(reply,&QNetworkReply::finished,[surl,reply,success,failed](){
			OnReply(surl,reply,success,failed);
		});
	}catch(std::exception&exc){
		Json::Value resp;
		log_warn("error while process {}: {}",surl,exc.what());
		if(failed)failed(resp,reply,exc.what());
	}
}
