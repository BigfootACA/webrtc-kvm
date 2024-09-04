/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef WEBRTC_CLIENT_SCREEN_INPUT_H
#define WEBRTC_CLIENT_SCREEN_INPUT_H
#include"../screen.h"
#include"lib/log.h"
#include"input/input.h"

class InputKeyMap{
	public:
		virtual ~InputKeyMap()=default;
		virtual InputKeyCode Translate(QKeyEvent*ev)=0;
};

class InputKeyMapFactory{
	public:
		inline virtual ~InputKeyMapFactory(){UnregisterSelf();}
		virtual std::string GetName()const=0;
		virtual std::vector<std::string>GetSupported()const=0;
		virtual std::shared_ptr<InputKeyMap>Create()const=0;
		static InputKeyMapFactory*FindFactoryByName(const std::string&name);
		static InputKeyMapFactory*FindFactoryBySupported(const std::string&plugin);
		static std::shared_ptr<InputKeyMap>CreateByName(const std::string&name);
		static std::shared_ptr<InputKeyMap>CreateBySupported(const std::string&plugin);
		static std::shared_ptr<InputKeyMap>CreateAuto();
	protected:
		void RegisterSelf();
		void UnregisterSelf();
};

class InputEventProcess{
	friend class VideoBox;
	public:
		InputEventProcess(WebRTCScreen*screen,std::shared_ptr<rtc::DataChannel>ch);
		enum EventType{
			PRESS,
			RELEASE,
			MOVE,
		};
		void SendInputEvent(InputEvent&ev);
		static std::shared_ptr<InputEventProcess>FindInput(QObject*begin);
	protected:
		void onOpen();
		void onClosed();
		void onError(std::string reason);
		void onMessage(rtc::message_variant msg);
		bool onKeyEvent(QKeyEvent*event,EventType type);
		bool onTabletEvent(QTabletEvent*event,EventType type);
		bool onMouseEvent(QMouseEvent*event,EventType type);
		bool onWheelEvent(QWheelEvent*event);
	private:
		WebRTCScreen*screen;
		std::shared_ptr<InputKeyMap>map;
		std::shared_ptr<rtc::DataChannel>ch;
};
#endif
