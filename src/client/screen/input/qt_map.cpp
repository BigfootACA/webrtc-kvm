/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"input.h"

class QtInputKeyMap:public InputKeyMap{
	public:
		InputKeyCode Translate(QKeyEvent*ev);
};

class QtInputKeyMapFactory:public InputKeyMapFactory{
	public:
		inline QtInputKeyMapFactory(){RegisterSelf();}
		inline std::string GetName()const final{return "qt-fallback";}
		inline std::vector<std::string>GetSupported()const final{return {};}
		inline std::shared_ptr<InputKeyMap>Create()const final{
			return std::make_shared<QtInputKeyMap>();
		}
};

cdecl_attr_used QtInputKeyMapFactory qt_fallback_key_map_factory;

InputKeyCode QtInputKeyMap::Translate(QKeyEvent*ev){
	switch(ev->key()){
		case Qt::Key::Key_Space:return KEY_CODE_SPACE;
		case Qt::Key::Key_Apostrophe:return KEY_CODE_APOSTROPHE;
		case Qt::Key::Key_Comma:return KEY_CODE_COMMA;
		case Qt::Key::Key_Period:return KEY_CODE_DOT;
		case Qt::Key::Key_Minus:return KEY_CODE_MINUS;
		case Qt::Key::Key_Slash:return KEY_CODE_SLASH;
		case Qt::Key::Key_0:return KEY_CODE_0;
		case Qt::Key::Key_1:return KEY_CODE_1;
		case Qt::Key::Key_2:return KEY_CODE_2;
		case Qt::Key::Key_3:return KEY_CODE_3;
		case Qt::Key::Key_4:return KEY_CODE_4;
		case Qt::Key::Key_5:return KEY_CODE_5;
		case Qt::Key::Key_6:return KEY_CODE_6;
		case Qt::Key::Key_7:return KEY_CODE_7;
		case Qt::Key::Key_8:return KEY_CODE_8;
		case Qt::Key::Key_9:return KEY_CODE_9;
		case Qt::Key::Key_Semicolon:return KEY_CODE_SEMICOLON;
		case Qt::Key::Key_Equal:return KEY_CODE_EQUAL;
		case Qt::Key::Key_Question:return KEY_CODE_QUESTION;
		case Qt::Key::Key_A:return KEY_CODE_A;
		case Qt::Key::Key_B:return KEY_CODE_B;
		case Qt::Key::Key_C:return KEY_CODE_C;
		case Qt::Key::Key_D:return KEY_CODE_D;
		case Qt::Key::Key_E:return KEY_CODE_E;
		case Qt::Key::Key_F:return KEY_CODE_F;
		case Qt::Key::Key_G:return KEY_CODE_G;
		case Qt::Key::Key_H:return KEY_CODE_H;
		case Qt::Key::Key_I:return KEY_CODE_I;
		case Qt::Key::Key_J:return KEY_CODE_J;
		case Qt::Key::Key_K:return KEY_CODE_K;
		case Qt::Key::Key_L:return KEY_CODE_L;
		case Qt::Key::Key_M:return KEY_CODE_M;
		case Qt::Key::Key_N:return KEY_CODE_N;
		case Qt::Key::Key_O:return KEY_CODE_O;
		case Qt::Key::Key_P:return KEY_CODE_P;
		case Qt::Key::Key_Q:return KEY_CODE_Q;
		case Qt::Key::Key_R:return KEY_CODE_R;
		case Qt::Key::Key_S:return KEY_CODE_S;
		case Qt::Key::Key_T:return KEY_CODE_T;
		case Qt::Key::Key_U:return KEY_CODE_U;
		case Qt::Key::Key_V:return KEY_CODE_V;
		case Qt::Key::Key_W:return KEY_CODE_W;
		case Qt::Key::Key_X:return KEY_CODE_X;
		case Qt::Key::Key_Y:return KEY_CODE_Y;
		case Qt::Key::Key_Z:return KEY_CODE_Z;
		case Qt::Key::Key_BracketLeft:return KEY_CODE_LEFTBRACE;
		case Qt::Key::Key_Backslash:return KEY_CODE_BACKSLASH;
		case Qt::Key::Key_BracketRight:return KEY_CODE_RIGHTBRACE;
		case Qt::Key::Key_Escape:return KEY_CODE_ESC;
		case Qt::Key::Key_Tab:return KEY_CODE_TAB;
		case Qt::Key::Key_Backspace:return KEY_CODE_BACKSPACE;
		case Qt::Key::Key_Return:return KEY_CODE_ENTER;
		case Qt::Key::Key_Enter:return KEY_CODE_ENTER;
		case Qt::Key::Key_Insert:return KEY_CODE_INSERT;
		case Qt::Key::Key_Delete:return KEY_CODE_DELETE;
		case Qt::Key::Key_Pause:return KEY_CODE_PAUSE;
		case Qt::Key::Key_Print:return KEY_CODE_PRINT;
		case Qt::Key::Key_Home:return KEY_CODE_HOME;
		case Qt::Key::Key_End:return KEY_CODE_END;
		case Qt::Key::Key_Left:return KEY_CODE_LEFT;
		case Qt::Key::Key_Up:return KEY_CODE_UP;
		case Qt::Key::Key_Right:return KEY_CODE_RIGHT;
		case Qt::Key::Key_Down:return KEY_CODE_DOWN;
		case Qt::Key::Key_PageUp:return KEY_CODE_PAGEUP;
		case Qt::Key::Key_PageDown:return KEY_CODE_PAGEDOWN;
		case Qt::Key::Key_Shift:return KEY_CODE_LEFTSHIFT;
		case Qt::Key::Key_Control:return KEY_CODE_LEFTCTRL;
		case Qt::Key::Key_Meta:return KEY_CODE_LEFTMETA;
		case Qt::Key::Key_Alt:return KEY_CODE_LEFTALT;
		case Qt::Key::Key_CapsLock:return KEY_CODE_CAPSLOCK;
		case Qt::Key::Key_NumLock:return KEY_CODE_NUMLOCK;
		case Qt::Key::Key_ScrollLock:return KEY_CODE_SCROLLLOCK;
		case Qt::Key::Key_F1:return KEY_CODE_F1;
		case Qt::Key::Key_F2:return KEY_CODE_F2;
		case Qt::Key::Key_F3:return KEY_CODE_F3;
		case Qt::Key::Key_F4:return KEY_CODE_F4;
		case Qt::Key::Key_F5:return KEY_CODE_F5;
		case Qt::Key::Key_F6:return KEY_CODE_F6;
		case Qt::Key::Key_F7:return KEY_CODE_F7;
		case Qt::Key::Key_F8:return KEY_CODE_F8;
		case Qt::Key::Key_F9:return KEY_CODE_F9;
		case Qt::Key::Key_F10:return KEY_CODE_F10;
		case Qt::Key::Key_F11:return KEY_CODE_F11;
		case Qt::Key::Key_F12:return KEY_CODE_F12;
		case Qt::Key::Key_F13:return KEY_CODE_F13;
		case Qt::Key::Key_F14:return KEY_CODE_F14;
		case Qt::Key::Key_F15:return KEY_CODE_F15;
		case Qt::Key::Key_F16:return KEY_CODE_F16;
		case Qt::Key::Key_F17:return KEY_CODE_F17;
		case Qt::Key::Key_F18:return KEY_CODE_F18;
		case Qt::Key::Key_F19:return KEY_CODE_F19;
		case Qt::Key::Key_F20:return KEY_CODE_F20;
		case Qt::Key::Key_F21:return KEY_CODE_F21;
		case Qt::Key::Key_F22:return KEY_CODE_F22;
		case Qt::Key::Key_F23:return KEY_CODE_F23;
		case Qt::Key::Key_F24:return KEY_CODE_F24;
		case Qt::Key::Key_Super_L:return KEY_CODE_LEFTMETA;
		case Qt::Key::Key_Super_R:return KEY_CODE_RIGHTMETA;
		case Qt::Key::Key_Menu:return KEY_CODE_MENU;
		case Qt::Key::Key_Help:return KEY_CODE_HELP;
		case Qt::Key::Key_Direction_L:return KEY_CODE_LEFT;
		case Qt::Key::Key_Direction_R:return KEY_CODE_RIGHT;
		case Qt::Key::Key_AltGr:return KEY_CODE_RIGHTALT;
		case Qt::Key::Key_Back:return KEY_CODE_BACK;
		case Qt::Key::Key_Forward:return KEY_CODE_FORWARD;
		case Qt::Key::Key_Stop:return KEY_CODE_STOP;
		case Qt::Key::Key_Refresh:return KEY_CODE_REFRESH;
		case Qt::Key::Key_VolumeDown:return KEY_CODE_VOLUMEDOWN;
		case Qt::Key::Key_VolumeMute:return KEY_CODE_MUTE;
		case Qt::Key::Key_VolumeUp:return KEY_CODE_VOLUMEUP;
		case Qt::Key::Key_BassBoost:return KEY_CODE_BASSBOOST;
		case Qt::Key::Key_MediaPlay:return KEY_CODE_PLAY;
		case Qt::Key::Key_MediaStop:return KEY_CODE_STOP;
		case Qt::Key::Key_MediaPrevious:return KEY_CODE_VIDEO_PREV;
		case Qt::Key::Key_MediaNext:return KEY_CODE_VIDEO_NEXT;
		case Qt::Key::Key_MediaRecord:return KEY_CODE_RECORD;
		case Qt::Key::Key_MediaPause:return KEY_CODE_PAUSE;
		case Qt::Key::Key_MediaTogglePlayPause:return KEY_CODE_PLAYPAUSE;
		case Qt::Key::Key_HomePage:return KEY_CODE_HOMEPAGE;
		case Qt::Key::Key_Search:return KEY_CODE_SEARCH;
		case Qt::Key::Key_Launch0:return KEY_CODE_PROG1;
		case Qt::Key::Key_Launch1:return KEY_CODE_PROG2;
		case Qt::Key::Key_Launch2:return KEY_CODE_PROG3;
		case Qt::Key::Key_Launch3:return KEY_CODE_PROG4;
		case Qt::Key::Key_MonBrightnessUp:return KEY_CODE_BRIGHTNESSUP;
		case Qt::Key::Key_MonBrightnessDown:return KEY_CODE_BRIGHTNESSDOWN;
		case Qt::Key::Key_PowerOff:return KEY_CODE_POWER;
		case Qt::Key::Key_WakeUp:return KEY_CODE_WAKEUP;
		case Qt::Key::Key_Eject:return KEY_CODE_EJECTCD;
		case Qt::Key::Key_ScreenSaver:return KEY_CODE_SCROLLLOCK;
		case Qt::Key::Key_WWW:return KEY_CODE_WWW;
		case Qt::Key::Key_Shop:return KEY_CODE_SHOP;
		case Qt::Key::Key_BrightnessAdjust:return KEY_CODE_BRIGHTNESS_CYCLE;
		case Qt::Key::Key_Finance:return KEY_CODE_FINANCE;
		case Qt::Key::Key_Close:return KEY_CODE_CLOSE;
		case Qt::Key::Key_Copy:return KEY_CODE_COPY;
		case Qt::Key::Key_Cut:return KEY_CODE_CUT;
		case Qt::Key::Key_Documents:return KEY_CODE_DOCUMENTS;
		case Qt::Key::Key_Paste:return KEY_CODE_PASTE;
		case Qt::Key::Key_Phone:return KEY_CODE_PHONE;
		case Qt::Key::Key_Reply:return KEY_CODE_REPLY;
		case Qt::Key::Key_Reload:return KEY_CODE_REFRESH;
		case Qt::Key::Key_Save:return KEY_CODE_SAVE;
		case Qt::Key::Key_Send:return KEY_CODE_SEND;
		case Qt::Key::Key_Xfer:return KEY_CODE_XFER;
		case Qt::Key::Key_WebCam:return KEY_CODE_CAMERA;
		case Qt::Key::Key_Battery:return KEY_CODE_BATTERY;
		case Qt::Key::Key_Bluetooth:return KEY_CODE_BLUETOOTH;
		case Qt::Key::Key_WLAN:return KEY_CODE_WLAN;
		case Qt::Key::Key_UWB:return KEY_CODE_UWB;
		case Qt::Key::Key_Hibernate:return KEY_CODE_SUSPEND;
		case Qt::Key::Key_Suspend:return KEY_CODE_SUSPEND;
		case Qt::Key::Key_MicMute:return KEY_CODE_MICMUTE;
		case Qt::Key::Key_New:return KEY_CODE_NEW;
		case Qt::Key::Key_Open:return KEY_CODE_OPEN;
		case Qt::Key::Key_Find:return KEY_CODE_FIND;
		case Qt::Key::Key_Undo:return KEY_CODE_UNDO;
		case Qt::Key::Key_Redo:return KEY_CODE_REDO;
		case Qt::Key::Key_Cancel:return KEY_CODE_CANCEL;
		case Qt::Key::Key_Sleep:return KEY_CODE_SLEEP;
		case Qt::Key::Key_Play:return KEY_CODE_PLAY;
		case Qt::Key::Key_Exit:return KEY_CODE_EXIT;
		case Qt::Key::Key_Camera:return KEY_CODE_CAMERA;
		default:return KEY_CODE_RESERVED;
	}
}
