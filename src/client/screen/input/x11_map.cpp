/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"input.h"
#include"lib/exception.h"
#include<X11/Xlib.h>
#include<X11/XKBlib.h>
#include<xkbcommon/xkbcommon-keysyms.h>

class X11InputKeyMap:public InputKeyMap{
	public:
		X11InputKeyMap();
		InputKeyCode Translate(QKeyEvent*ev);
	private:
		Display*display=nullptr;
};

class X11InputKeyMapFactory:public InputKeyMapFactory{
	public:
		inline X11InputKeyMapFactory(){RegisterSelf();}
		inline std::string GetName()const final{return "x11";}
		inline std::vector<std::string>GetSupported()const final{return {"xcb"};}
		inline std::shared_ptr<InputKeyMap>Create()const final{
			return std::make_shared<X11InputKeyMap>();
		}
};

cdecl_attr_used X11InputKeyMapFactory x11_key_map_factory;

X11InputKeyMap::X11InputKeyMap(){
	if(!(display=XOpenDisplay(nullptr)))
		throw RuntimeError("open x11 display failed");
}

InputKeyCode X11InputKeyMap::Translate(QKeyEvent*ev){
	switch(XkbKeycodeToKeysym(display,ev->nativeScanCode(),0,0)){
		case XKB_KEY_Escape:return KEY_CODE_ESC;
		case XKB_KEY_minus:return KEY_CODE_MINUS;
		case XKB_KEY_equal:return KEY_CODE_EQUAL;
		case XKB_KEY_BackSpace:return KEY_CODE_BACKSPACE;
		case XKB_KEY_Tab:return KEY_CODE_TAB;
		case XKB_KEY_1:return KEY_CODE_1;
		case XKB_KEY_2:return KEY_CODE_2;
		case XKB_KEY_3:return KEY_CODE_3;
		case XKB_KEY_4:return KEY_CODE_4;
		case XKB_KEY_5:return KEY_CODE_5;
		case XKB_KEY_6:return KEY_CODE_6;
		case XKB_KEY_7:return KEY_CODE_7;
		case XKB_KEY_8:return KEY_CODE_8;
		case XKB_KEY_9:return KEY_CODE_9;
		case XKB_KEY_0:return KEY_CODE_0;
		case XKB_KEY_A:return KEY_CODE_A;
		case XKB_KEY_B:return KEY_CODE_B;
		case XKB_KEY_C:return KEY_CODE_C;
		case XKB_KEY_D:return KEY_CODE_D;
		case XKB_KEY_E:return KEY_CODE_E;
		case XKB_KEY_F:return KEY_CODE_F;
		case XKB_KEY_G:return KEY_CODE_G;
		case XKB_KEY_H:return KEY_CODE_H;
		case XKB_KEY_I:return KEY_CODE_I;
		case XKB_KEY_J:return KEY_CODE_J;
		case XKB_KEY_K:return KEY_CODE_K;
		case XKB_KEY_L:return KEY_CODE_L;
		case XKB_KEY_M:return KEY_CODE_M;
		case XKB_KEY_N:return KEY_CODE_N;
		case XKB_KEY_O:return KEY_CODE_O;
		case XKB_KEY_P:return KEY_CODE_P;
		case XKB_KEY_Q:return KEY_CODE_Q;
		case XKB_KEY_R:return KEY_CODE_R;
		case XKB_KEY_S:return KEY_CODE_S;
		case XKB_KEY_T:return KEY_CODE_T;
		case XKB_KEY_U:return KEY_CODE_U;
		case XKB_KEY_V:return KEY_CODE_V;
		case XKB_KEY_W:return KEY_CODE_W;
		case XKB_KEY_X:return KEY_CODE_X;
		case XKB_KEY_Y:return KEY_CODE_Y;
		case XKB_KEY_Z:return KEY_CODE_Z;
		case XKB_KEY_a:return KEY_CODE_A;
		case XKB_KEY_b:return KEY_CODE_B;
		case XKB_KEY_c:return KEY_CODE_C;
		case XKB_KEY_d:return KEY_CODE_D;
		case XKB_KEY_e:return KEY_CODE_E;
		case XKB_KEY_f:return KEY_CODE_F;
		case XKB_KEY_g:return KEY_CODE_G;
		case XKB_KEY_h:return KEY_CODE_H;
		case XKB_KEY_i:return KEY_CODE_I;
		case XKB_KEY_j:return KEY_CODE_J;
		case XKB_KEY_k:return KEY_CODE_K;
		case XKB_KEY_l:return KEY_CODE_L;
		case XKB_KEY_m:return KEY_CODE_M;
		case XKB_KEY_n:return KEY_CODE_N;
		case XKB_KEY_o:return KEY_CODE_O;
		case XKB_KEY_p:return KEY_CODE_P;
		case XKB_KEY_q:return KEY_CODE_Q;
		case XKB_KEY_r:return KEY_CODE_R;
		case XKB_KEY_s:return KEY_CODE_S;
		case XKB_KEY_t:return KEY_CODE_T;
		case XKB_KEY_u:return KEY_CODE_U;
		case XKB_KEY_v:return KEY_CODE_V;
		case XKB_KEY_w:return KEY_CODE_W;
		case XKB_KEY_x:return KEY_CODE_X;
		case XKB_KEY_y:return KEY_CODE_Y;
		case XKB_KEY_z:return KEY_CODE_Z;
		case XKB_KEY_bracketleft:return KEY_CODE_LEFTBRACE;
		case XKB_KEY_bracketright:return KEY_CODE_RIGHTBRACE;
		case XKB_KEY_braceleft:return KEY_CODE_LEFTBRACE;
		case XKB_KEY_braceright:return KEY_CODE_RIGHTBRACE;
		case XKB_KEY_Return:return KEY_CODE_ENTER;
		case XKB_KEY_Control_L:return KEY_CODE_LEFTCTRL;
		case XKB_KEY_semicolon:return KEY_CODE_SEMICOLON;
		case XKB_KEY_apostrophe:return KEY_CODE_APOSTROPHE;
		case XKB_KEY_grave:return KEY_CODE_GRAVE;
		case XKB_KEY_Shift_L:return KEY_CODE_LEFTSHIFT;
		case XKB_KEY_backslash:return KEY_CODE_BACKSLASH;
		case XKB_KEY_comma:return KEY_CODE_COMMA;
		case XKB_KEY_period:return KEY_CODE_DOT;
		case XKB_KEY_slash:return KEY_CODE_SLASH;
		case XKB_KEY_Shift_R:return KEY_CODE_RIGHTSHIFT;
		case XKB_KEY_KP_Multiply:return KEY_CODE_KPASTERISK;
		case XKB_KEY_Alt_L:return KEY_CODE_LEFTALT;
		case XKB_KEY_space:return KEY_CODE_SPACE;
		case XKB_KEY_Caps_Lock:return KEY_CODE_CAPSLOCK;
		case XKB_KEY_F1:return KEY_CODE_F1;
		case XKB_KEY_F2:return KEY_CODE_F2;
		case XKB_KEY_F3:return KEY_CODE_F3;
		case XKB_KEY_F4:return KEY_CODE_F4;
		case XKB_KEY_F5:return KEY_CODE_F5;
		case XKB_KEY_F6:return KEY_CODE_F6;
		case XKB_KEY_F7:return KEY_CODE_F7;
		case XKB_KEY_F8:return KEY_CODE_F8;
		case XKB_KEY_F9:return KEY_CODE_F9;
		case XKB_KEY_F10:return KEY_CODE_F10;
		case XKB_KEY_Num_Lock:return KEY_CODE_NUMLOCK;
		case XKB_KEY_Scroll_Lock:return KEY_CODE_SCROLLLOCK;
		case XKB_KEY_KP_7:return KEY_CODE_KP7;
		case XKB_KEY_KP_8:return KEY_CODE_KP8;
		case XKB_KEY_KP_9:return KEY_CODE_KP9;
		case XKB_KEY_KP_Subtract:return KEY_CODE_KPMINUS;
		case XKB_KEY_KP_4:return KEY_CODE_KP4;
		case XKB_KEY_KP_5:return KEY_CODE_KP5;
		case XKB_KEY_KP_6:return KEY_CODE_KP6;
		case XKB_KEY_KP_Add:return KEY_CODE_KPPLUS;
		case XKB_KEY_KP_1:return KEY_CODE_KP1;
		case XKB_KEY_KP_2:return KEY_CODE_KP2;
		case XKB_KEY_KP_3:return KEY_CODE_KP3;
		case XKB_KEY_KP_0:return KEY_CODE_KP0;
		case XKB_KEY_KP_Decimal:return KEY_CODE_KPDOT;
		case XKB_KEY_Print:return KEY_CODE_PRTSC;
		case XKB_KEY_F11:return KEY_CODE_F11;
		case XKB_KEY_F12:return KEY_CODE_F12;
		case XKB_KEY_KP_Enter:return KEY_CODE_KPENTER;
		case XKB_KEY_Control_R:return KEY_CODE_RIGHTCTRL;
		case XKB_KEY_KP_Divide:return KEY_CODE_KPSLASH;
		case XKB_KEY_Sys_Req:return KEY_CODE_SYSRQ;
		case XKB_KEY_Alt_R:return KEY_CODE_RIGHTALT;
		case XKB_KEY_Linefeed:return KEY_CODE_LINEFEED;
		case XKB_KEY_Home:return KEY_CODE_HOME;
		case XKB_KEY_Up:return KEY_CODE_UP;
		case XKB_KEY_Page_Up:return KEY_CODE_PAGEUP;
		case XKB_KEY_Left:return KEY_CODE_LEFT;
		case XKB_KEY_Right:return KEY_CODE_RIGHT;
		case XKB_KEY_End:return KEY_CODE_END;
		case XKB_KEY_Down:return KEY_CODE_DOWN;
		case XKB_KEY_Page_Down:return KEY_CODE_PAGEDOWN;
		case XKB_KEY_Insert:return KEY_CODE_INSERT;
		case XKB_KEY_Delete:return KEY_CODE_DELETE;
		case XKB_KEY_macron:return KEY_CODE_MACRO;
		case XKB_KEY_XF86AudioMute:return KEY_CODE_MUTE;
		case XKB_KEY_XF86AudioLowerVolume:return KEY_CODE_VOLUMEDOWN;
		case XKB_KEY_XF86AudioRaiseVolume:return KEY_CODE_VOLUMEUP;
		case XKB_KEY_XF86PowerDown:return KEY_CODE_POWER;
		case XKB_KEY_XF86PowerOff:return KEY_CODE_POWER;
		case XKB_KEY_KP_Equal:return KEY_CODE_KPEQUAL;
		case XKB_KEY_Pause:return KEY_CODE_PAUSE;
		case XKB_KEY_KP_Separator:return KEY_CODE_KPCOMMA;
		case XKB_KEY_Super_L:return KEY_CODE_LEFTMETA;
		case XKB_KEY_Super_R:return KEY_CODE_RIGHTMETA;
		case XKB_KEY_XF86Stop:return KEY_CODE_STOP;
		case XKB_KEY_Undo:return KEY_CODE_UNDO;
		case XKB_KEY_XF86Copy:return KEY_CODE_COPY;
		case XKB_KEY_XF86Open:return KEY_CODE_OPEN;
		case XKB_KEY_XF86Paste:return KEY_CODE_PASTE;
		case XKB_KEY_Find:return KEY_CODE_FIND;
		case XKB_KEY_XF86Cut:return KEY_CODE_CUT;
		case XKB_KEY_Help:return KEY_CODE_HELP;
		case XKB_KEY_Menu:return KEY_CODE_MENU;
		case XKB_KEY_XF86Calculater:return KEY_CODE_CALC;
		case XKB_KEY_XF86Sleep:return KEY_CODE_SLEEP;
		case XKB_KEY_XF86WakeUp:return KEY_CODE_WAKEUP;
		case XKB_KEY_XF86Explorer:return KEY_CODE_FILE;
		case XKB_KEY_XF86Xfer:return KEY_CODE_XFER;
		case XKB_KEY_XF86WWW:return KEY_CODE_WWW;
		case XKB_KEY_XF86DOS:return KEY_CODE_MSDOS;
		case XKB_KEY_XF86RotateWindows:return KEY_CODE_CYCLEWINDOWS;
		case XKB_KEY_XF86Mail:return KEY_CODE_MAIL;
		case XKB_KEY_XF86Favorites:return KEY_CODE_BOOKMARKS;
		case XKB_KEY_XF86MyComputer:return KEY_CODE_COMPUTER;
		case XKB_KEY_XF86Back:return KEY_CODE_BACK;
		case XKB_KEY_XF86Forward:return KEY_CODE_FORWARD;
		case XKB_KEY_XF86Eject:return KEY_CODE_EJECTCD;
		case XKB_KEY_XF86AudioNext:return KEY_CODE_NEXTSONG;
		case XKB_KEY_XF86AudioPrev:return KEY_CODE_PREVIOUSSONG;
		case XKB_KEY_XF86AudioStop:return KEY_CODE_STOPCD;
		case XKB_KEY_XF86AudioRecord:return KEY_CODE_RECORD;
		case XKB_KEY_XF86AudioRewind:return KEY_CODE_REWIND;
		case XKB_KEY_XF86Phone:return KEY_CODE_PHONE;
		case XKB_KEY_XF86HomePage:return KEY_CODE_HOMEPAGE;
		case XKB_KEY_XF86Refresh:return KEY_CODE_REFRESH;
		case XKB_KEY_Cancel:return KEY_CODE_EXIT;
		case XKB_KEY_XF86ScrollUp:return KEY_CODE_SCROLLUP;
		case XKB_KEY_XF86ScrollDown:return KEY_CODE_SCROLLDOWN;
		case XKB_KEY_parenleft:return KEY_CODE_KPLEFTPAREN;
		case XKB_KEY_parenright:return KEY_CODE_KPRIGHTPAREN;
		case XKB_KEY_XF86New:return KEY_CODE_NEW;
		case XKB_KEY_Redo:return KEY_CODE_REDO;
		case XKB_KEY_F13:return KEY_CODE_F13;
		case XKB_KEY_F14:return KEY_CODE_F14;
		case XKB_KEY_F15:return KEY_CODE_F15;
		case XKB_KEY_F16:return KEY_CODE_F16;
		case XKB_KEY_F17:return KEY_CODE_F17;
		case XKB_KEY_F18:return KEY_CODE_F18;
		case XKB_KEY_F19:return KEY_CODE_F19;
		case XKB_KEY_F20:return KEY_CODE_F20;
		case XKB_KEY_F21:return KEY_CODE_F21;
		case XKB_KEY_F22:return KEY_CODE_F22;
		case XKB_KEY_F23:return KEY_CODE_F23;
		case XKB_KEY_F24:return KEY_CODE_F24;
		case XKB_KEY_XF86AudioPlay:return KEY_CODE_PLAYCD;
		case XKB_KEY_XF86AudioPause:return KEY_CODE_PAUSECD;
		case XKB_KEY_XF86Launch3:return KEY_CODE_PROG3;
		case XKB_KEY_XF86Launch4:return KEY_CODE_PROG4;
		case XKB_KEY_XF86LaunchB:return KEY_CODE_DASHBOARD;
		case XKB_KEY_XF86Suspend:return KEY_CODE_SUSPEND;
		case XKB_KEY_XF86Close:return KEY_CODE_CLOSE;
		case XKB_KEY_XF86WebCam:return KEY_CODE_CAMERA;
		case XKB_KEY_XF86Audio:return KEY_CODE_SOUND;
		case XKB_KEY_question:return KEY_CODE_QUESTION;
		case XKB_KEY_XF86Finance:return KEY_CODE_FINANCE;
		case XKB_KEY_XF86Shop:return KEY_CODE_SHOP;
		case XKB_KEY_XF86MonBrightnessDown:return KEY_CODE_BRIGHTNESSDOWN;
		case XKB_KEY_XF86MonBrightnessUp:return KEY_CODE_BRIGHTNESSUP;
		case XKB_KEY_XF86AudioMedia:return KEY_CODE_MEDIA;
		case XKB_KEY_XF86Display:return KEY_CODE_SWITCHVIDEOMODE;
		case XKB_KEY_XF86KbdLightOnOff:return KEY_CODE_KBDILLUMTOGGLE;
		case XKB_KEY_XF86KbdBrightnessDown:return KEY_CODE_KBDILLUMDOWN;
		case XKB_KEY_XF86KbdBrightnessUp:return KEY_CODE_KBDILLUMUP;
		case XKB_KEY_XF86Send:return KEY_CODE_SEND;
		case XKB_KEY_XF86Reply:return KEY_CODE_REPLY;
		case XKB_KEY_XF86MailForward:return KEY_CODE_FORWARDMAIL;
		case XKB_KEY_XF86Save:return KEY_CODE_SAVE;
		case XKB_KEY_XF86Documents:return KEY_CODE_DOCUMENTS;
		case XKB_KEY_XF86Battery:return KEY_CODE_BATTERY;
		case XKB_KEY_XF86Bluetooth:return KEY_CODE_BLUETOOTH;
		case XKB_KEY_XF86WLAN:return KEY_CODE_WLAN;
		case XKB_KEY_XF86UWB:return KEY_CODE_UWB;
		case XKB_KEY_XF86Next_VMode:return KEY_CODE_VIDEO_NEXT;
		case XKB_KEY_XF86Prev_VMode:return KEY_CODE_VIDEO_PREV;
		case XKB_KEY_XF86MonBrightnessCycle:return KEY_CODE_BRIGHTNESS_CYCLE;
		case XKB_KEY_XF86BrightnessAuto:return KEY_CODE_BRIGHTNESS_AUTO;
		case XKB_KEY_XF86DisplayOff:return KEY_CODE_DISPLAY_OFF;
		case XKB_KEY_XF86WWAN:return KEY_CODE_WWAN;
		case XKB_KEY_XF86RFKill:return KEY_CODE_RFKILL;
		case XKB_KEY_XF86AudioMicMute:return KEY_CODE_MICMUTE;
		default:return KEY_CODE_RESERVED;
	}
}
