/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef WEBRTC_CLIENT_SCREEN_H
#define WEBRTC_CLIENT_SCREEN_H
#include<QVBoxLayout>
#include<QMainWindow>
#include<QMenuBar>
#include<QInputDialog>
#include<QStatusBar>
#include<QMessageBox>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QResizeEvent>
#include<QSettings>
#include<QMetaEnum>
#include<QTimer>
#include<QLabel>
#include<QUrl>
#include<rtc/rtc.hpp>
#include<json/json.h>
#include"algo/uuid.h"
#include"defines.h"

class VideoProcess;
class InputEventProcess;

class VideoBox:public QLabel{
	public:
		explicit VideoBox(QWidget*parent=nullptr);
	protected:
		void keyPressEvent(QKeyEvent*event)override;
		void keyReleaseEvent(QKeyEvent*event)override;
		void mousePressEvent(QMouseEvent*event)override;
		void mouseReleaseEvent(QMouseEvent*event)override;
		void mouseMoveEvent(QMouseEvent*event)override;
		#if QT_CONFIG(wheelevent)
		void wheelEvent(QWheelEvent*event)override;
		#endif
};

class WebRTCScreen:public QMainWindow{
	friend class VideoBox;
	friend class LoginContext;
	friend class VideoProcess;
	friend class InputEventProcess;
	Q_OBJECT
	public:
		explicit WebRTCScreen(QWidget*parent=nullptr);
		~WebRTCScreen()override;
		void Retranslate();
		void InitWebRTC();
		void Handshake();
		void Connect();
		void Disconnect();
		void Logout();
		void ChooseScaleFactor();
		void SubmitJob(std::function<void()>job,Qt::ConnectionType type);
		void TryLogin(
			std::function<void()>success,
			std::function<void(const std::string&)>failed=[](auto){}
		);
		void HttpCall(
			const std::string&path,
			Json::Value body={},
			std::function<void(Json::Value&,QNetworkReply*)>success=[](auto,auto){},
			std::function<void(Json::Value&,QNetworkReply*,const std::string&)>failed=[](auto,auto,auto){}
		);
		struct Counter{
			volatile uint64_t frames;
			volatile uint64_t packets;
			volatile uint64_t bytes;
		};
		enum Status{
			Disconnected,
			Authenticating,
			Initializing,
			Connecting,
			Connected,
		};
		Q_ENUM(Status);
		static constexpr int scale_factors[]{200,175,150,125,100,75,50,25};
	protected slots:
		void onStateChange(rtc::PeerConnection::State state);
		void onGatheringStateChange(rtc::PeerConnection::GatheringState state);
		void onLocalDescription(rtc::Description desc);
		void onTrack(std::shared_ptr<rtc::Track>track);
		void onVideoTrack(std::shared_ptr<rtc::Track>track);
		void onDataChannel(std::shared_ptr<rtc::DataChannel>ch);
		void onInputChannel(std::shared_ptr<rtc::DataChannel>ch);
		void onEventChannel(std::shared_ptr<rtc::DataChannel>ch);
		void onEventMessage(Json::Value msg);
		void onEventResizeMessage(Json::Value msg);
		void onJob(std::function<void()>job);
		void onResizeVideo(QResizeEvent*event);
		void onScaleFactorTriggered(int factor);
		void onActionConnectTriggered(bool);
		void onActionDisconnectTriggered(bool);
		void onActionLogoutTriggered(bool);
		void onTimerTimeout();
	protected:
		void resizeEvent(QResizeEvent*event)override;
		void enterEvent(QEnterEvent*event)override;
		std::string server;
		UUID webrtc_id;
		UUID token;
		int scale_factor=100;
		bool scale_factor_changed=false;
		QNetworkAccessManager net;
		QSettings settings;
		Counter current{},last{};
		std::shared_ptr<rtc::DataChannel>event_ch=nullptr;
	private:
		std::shared_ptr<rtc::PeerConnection>pc=nullptr;
		QSize outbox_size,video_size;
		QTimer*timer;
		VideoBox*video;
		QWidget*central;
		QMenuBar*menu_bar;
		QStatusBar*status_bar;
		QMenu*menu_connection;
		QAction*action_connect;
		QAction*action_disconnect;
		QAction*action_logout;
		QMenu*menu_scale;
		QAction*action_scale_factor[ARRAY_SIZE(scale_factors)];
		QLabel*status_status;
		QLabel*status_frames;
		QLabel*status_packets;
		QLabel*status_bytes;
		std::shared_ptr<InputEventProcess>input_ch=nullptr;
		std::shared_ptr<VideoProcess>video_process=nullptr;
		Json::Value auth_info;
		Status status=Disconnected;
		void setStatus(Status s);
};
#endif
