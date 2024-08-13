/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef INPUT_H
#define INPUT_H
#include<string>
#include<cstdint>

#define EVENT_MAGIC "!IN"

enum InputEventType{
	EVENT_UNKNOWN       = 0,
	EVENT_KEY_DOWN      = 1,
	EVENT_KEY_UP        = 2,
	EVENT_MOUSE_WHEEL   = 3,
	EVENT_ABSOLUTE_DOWN = 4,
	EVENT_ABSOLUTE_UP   = 5,
	EVENT_ABSOLUTE_MOVE = 6,
	EVENT_RELATIVE_DOWN = 7,
	EVENT_RELATIVE_UP   = 8,
	EVENT_RELATIVE_MOVE = 9,
	EVENT_TOUCH_START   = 10,
	EVENT_TOUCH_MOVE    = 11,
	EVENT_TOUCH_END     = 12,
	EVENT_TOUCH_CANCEL  = 13,
};

enum InputKeyCode{
	KEY_CODE_RESERVED         = 0,
	KEY_CODE_ESC              = 1,
	KEY_CODE_1                = 2,
	KEY_CODE_2                = 3,
	KEY_CODE_3                = 4,
	KEY_CODE_4                = 5,
	KEY_CODE_5                = 6,
	KEY_CODE_6                = 7,
	KEY_CODE_7                = 8,
	KEY_CODE_8                = 9,
	KEY_CODE_9                = 10,
	KEY_CODE_0                = 11,
	KEY_CODE_MINUS            = 12,
	KEY_CODE_EQUAL            = 13,
	KEY_CODE_BACKSPACE        = 14,
	KEY_CODE_TAB              = 15,
	KEY_CODE_Q                = 16,
	KEY_CODE_W                = 17,
	KEY_CODE_E                = 18,
	KEY_CODE_R                = 19,
	KEY_CODE_T                = 20,
	KEY_CODE_Y                = 21,
	KEY_CODE_U                = 22,
	KEY_CODE_I                = 23,
	KEY_CODE_O                = 24,
	KEY_CODE_P                = 25,
	KEY_CODE_LEFTBRACE        = 26,
	KEY_CODE_RIGHTBRACE       = 27,
	KEY_CODE_ENTER            = 28,
	KEY_CODE_LEFTCTRL         = 29,
	KEY_CODE_A                = 30,
	KEY_CODE_S                = 31,
	KEY_CODE_D                = 32,
	KEY_CODE_F                = 33,
	KEY_CODE_G                = 34,
	KEY_CODE_H                = 35,
	KEY_CODE_J                = 36,
	KEY_CODE_K                = 37,
	KEY_CODE_L                = 38,
	KEY_CODE_SEMICOLON        = 39,
	KEY_CODE_APOSTROPHE       = 40,
	KEY_CODE_GRAVE            = 41,
	KEY_CODE_LEFTSHIFT        = 42,
	KEY_CODE_BACKSLASH        = 43,
	KEY_CODE_Z                = 44,
	KEY_CODE_X                = 45,
	KEY_CODE_C                = 46,
	KEY_CODE_V                = 47,
	KEY_CODE_B                = 48,
	KEY_CODE_N                = 49,
	KEY_CODE_M                = 50,
	KEY_CODE_COMMA            = 51,
	KEY_CODE_DOT              = 52,
	KEY_CODE_SLASH            = 53,
	KEY_CODE_RIGHTSHIFT       = 54,
	KEY_CODE_KPASTERISK       = 55,
	KEY_CODE_LEFTALT          = 56,
	KEY_CODE_SPACE            = 57,
	KEY_CODE_CAPSLOCK         = 58,
	KEY_CODE_F1               = 59,
	KEY_CODE_F2               = 60,
	KEY_CODE_F3               = 61,
	KEY_CODE_F4               = 62,
	KEY_CODE_F5               = 63,
	KEY_CODE_F6               = 64,
	KEY_CODE_F7               = 65,
	KEY_CODE_F8               = 66,
	KEY_CODE_F9               = 67,
	KEY_CODE_F10              = 68,
	KEY_CODE_NUMLOCK          = 69,
	KEY_CODE_SCROLLLOCK       = 70,
	KEY_CODE_KP7              = 71,
	KEY_CODE_KP8              = 72,
	KEY_CODE_KP9              = 73,
	KEY_CODE_KPMINUS          = 74,
	KEY_CODE_KP4              = 75,
	KEY_CODE_KP5              = 76,
	KEY_CODE_KP6              = 77,
	KEY_CODE_KPPLUS           = 78,
	KEY_CODE_KP1              = 79,
	KEY_CODE_KP2              = 80,
	KEY_CODE_KP3              = 81,
	KEY_CODE_KP0              = 82,
	KEY_CODE_KPDOT            = 83,
	KEY_CODE_PRTSC            = 84,
	KEY_CODE_F11              = 87,
	KEY_CODE_F12              = 88,
	KEY_CODE_RO               = 89,
	KEY_CODE_KPENTER          = 96,
	KEY_CODE_RIGHTCTRL        = 97,
	KEY_CODE_KPSLASH          = 98,
	KEY_CODE_SYSRQ            = 99,
	KEY_CODE_RIGHTALT         = 100,
	KEY_CODE_LINEFEED         = 101,
	KEY_CODE_HOME             = 102,
	KEY_CODE_UP               = 103,
	KEY_CODE_PAGEUP           = 104,
	KEY_CODE_LEFT             = 105,
	KEY_CODE_RIGHT            = 106,
	KEY_CODE_END              = 107,
	KEY_CODE_DOWN             = 108,
	KEY_CODE_PAGEDOWN         = 109,
	KEY_CODE_INSERT           = 110,
	KEY_CODE_DELETE           = 111,
	KEY_CODE_MACRO            = 112,
	KEY_CODE_MUTE             = 113,
	KEY_CODE_VOLUMEDOWN       = 114,
	KEY_CODE_VOLUMEUP         = 115,
	KEY_CODE_POWER            = 116,
	KEY_CODE_KPEQUAL          = 117,
	KEY_CODE_KPPLUSMINUS      = 118,
	KEY_CODE_PAUSE            = 119,
	KEY_CODE_SCALE            = 120,
	KEY_CODE_KPCOMMA          = 121,
	KEY_CODE_LEFTMETA         = 125,
	KEY_CODE_RIGHTMETA        = 126,
	KEY_CODE_COMPOSE          = 127,
	KEY_CODE_STOP             = 128,
	KEY_CODE_AGAIN            = 129,
	KEY_CODE_PROPS            = 130,
	KEY_CODE_UNDO             = 131,
	KEY_CODE_FRONT            = 132,
	KEY_CODE_COPY             = 133,
	KEY_CODE_OPEN             = 134,
	KEY_CODE_PASTE            = 135,
	KEY_CODE_FIND             = 136,
	KEY_CODE_CUT              = 137,
	KEY_CODE_HELP             = 138,
	KEY_CODE_MENU             = 139,
	KEY_CODE_CALC             = 140,
	KEY_CODE_SETUP            = 141,
	KEY_CODE_SLEEP            = 142,
	KEY_CODE_WAKEUP           = 143,
	KEY_CODE_FILE             = 144,
	KEY_CODE_SENDFILE         = 145,
	KEY_CODE_DELETEFILE       = 146,
	KEY_CODE_XFER             = 147,
	KEY_CODE_PROG1            = 148,
	KEY_CODE_PROG2            = 149,
	KEY_CODE_WWW              = 150,
	KEY_CODE_MSDOS            = 151,
	KEY_CODE_SCREENLOCK       = 152,
	KEY_CODE_DIRECTION        = 153,
	KEY_CODE_CYCLEWINDOWS     = 154,
	KEY_CODE_MAIL             = 155,
	KEY_CODE_BOOKMARKS        = 156,
	KEY_CODE_COMPUTER         = 157,
	KEY_CODE_BACK             = 158,
	KEY_CODE_FORWARD          = 159,
	KEY_CODE_CLOSECD          = 160,
	KEY_CODE_EJECTCD          = 161,
	KEY_CODE_EJECTCLOSECD     = 162,
	KEY_CODE_NEXTSONG         = 163,
	KEY_CODE_PLAYPAUSE        = 164,
	KEY_CODE_PREVIOUSSONG     = 165,
	KEY_CODE_STOPCD           = 166,
	KEY_CODE_RECORD           = 167,
	KEY_CODE_REWIND           = 168,
	KEY_CODE_PHONE            = 169,
	KEY_CODE_ISO              = 170,
	KEY_CODE_CONFIG           = 171,
	KEY_CODE_HOMEPAGE         = 172,
	KEY_CODE_REFRESH          = 173,
	KEY_CODE_EXIT             = 174,
	KEY_CODE_MOVE             = 175,
	KEY_CODE_EDIT             = 176,
	KEY_CODE_SCROLLUP         = 177,
	KEY_CODE_SCROLLDOWN       = 178,
	KEY_CODE_KPLEFTPAREN      = 179,
	KEY_CODE_KPRIGHTPAREN     = 180,
	KEY_CODE_NEW              = 181,
	KEY_CODE_REDO             = 182,
	KEY_CODE_F13              = 183,
	KEY_CODE_F14              = 184,
	KEY_CODE_F15              = 185,
	KEY_CODE_F16              = 186,
	KEY_CODE_F17              = 187,
	KEY_CODE_F18              = 188,
	KEY_CODE_F19              = 189,
	KEY_CODE_F20              = 190,
	KEY_CODE_F21              = 191,
	KEY_CODE_F22              = 192,
	KEY_CODE_F23              = 193,
	KEY_CODE_F24              = 194,
	KEY_CODE_PLAYCD           = 200,
	KEY_CODE_PAUSECD          = 201,
	KEY_CODE_PROG3            = 202,
	KEY_CODE_PROG4            = 203,
	KEY_CODE_DASHBOARD        = 204,
	KEY_CODE_SUSPEND          = 205,
	KEY_CODE_CLOSE            = 206,
	KEY_CODE_PLAY             = 207,
	KEY_CODE_FASTFORWARD      = 208,
	KEY_CODE_BASSBOOST        = 209,
	KEY_CODE_PRINT            = 210,
	KEY_CODE_HP               = 211,
	KEY_CODE_CAMERA           = 212,
	KEY_CODE_SOUND            = 213,
	KEY_CODE_QUESTION         = 214,
	KEY_CODE_EMAIL            = 215,
	KEY_CODE_CHAT             = 216,
	KEY_CODE_SEARCH           = 217,
	KEY_CODE_CONNECT          = 218,
	KEY_CODE_FINANCE          = 219,
	KEY_CODE_SPORT            = 220,
	KEY_CODE_SHOP             = 221,
	KEY_CODE_ALTERASE         = 222,
	KEY_CODE_CANCEL           = 223,
	KEY_CODE_BRIGHTNESSDOWN   = 224,
	KEY_CODE_BRIGHTNESSUP     = 225,
	KEY_CODE_MEDIA            = 226,
	KEY_CODE_SWITCHVIDEOMODE  = 227,
	KEY_CODE_KBDILLUMTOGGLE   = 228,
	KEY_CODE_KBDILLUMDOWN     = 229,
	KEY_CODE_KBDILLUMUP       = 230,
	KEY_CODE_SEND             = 231,
	KEY_CODE_REPLY            = 232,
	KEY_CODE_FORWARDMAIL      = 233,
	KEY_CODE_SAVE             = 234,
	KEY_CODE_DOCUMENTS        = 235,
	KEY_CODE_BATTERY          = 236,
	KEY_CODE_BLUETOOTH        = 237,
	KEY_CODE_WLAN             = 238,
	KEY_CODE_UWB              = 239,
	KEY_CODE_UNKNOWN          = 240,
	KEY_CODE_VIDEO_NEXT       = 241,
	KEY_CODE_VIDEO_PREV       = 242,
	KEY_CODE_BRIGHTNESS_CYCLE = 243,
	KEY_CODE_BRIGHTNESS_AUTO  = 244,
	KEY_CODE_DISPLAY_OFF      = 245,
	KEY_CODE_WWAN             = 246,
	KEY_CODE_RFKILL           = 247,
	KEY_CODE_MICMUTE          = 248,
};

enum InputMouseButton{
	BTN_MOUSE_LEFT  = 1,
	BTN_MOUSE_WHEEL = 2,
	BTN_MOUSE_RIGHT = 3,
};

struct webrtc_kvm;

struct InputEventKey{
	InputKeyCode code:16;
};

struct InputEventMouseRelative{
	int16_t delta_x;
	int16_t delta_y;
	InputMouseButton btn:8;
};

struct InputEventMouseAbsolute{
	uint16_t pos_x;
	uint16_t pos_y;
	InputMouseButton btn:8;
};

struct InputEventTouch{
	uint16_t pos_x;
	uint16_t pos_y;
	uint8_t finger;
};

struct InputEventWheel{
	int16_t delta_x;
	int16_t delta_y;
	int16_t delta_z;
};

struct InputEvent{
	char magic[3];
	InputEventType type:8;
	union{
		InputEventKey key;
		InputEventMouseAbsolute absolute;
		InputEventMouseRelative relative;
		InputEventTouch touch;
		InputEventWheel wheel;
	};
};

struct WebKeyMap{
	InputKeyCode code;
	const char*name;
};

extern WebKeyMap WebKeyMapData[];
extern void InputEventKeyPrint(std::string prefix,const InputEventKey*ev);
extern void InputEventMouseAbsolutePrint(std::string prefix,const InputEventMouseAbsolute*ev);
extern void InputEventMouseRelativePrint(std::string prefix,const InputEventMouseRelative*ev);
extern void InputEventTouchPrint(std::string prefix,const InputEventTouch*ev);
extern void InputEventWheelPrint(std::string prefix,const InputEventWheel*ev);
extern void InputEventPrint(std::string prefix,const InputEvent*ev);
extern const char*InputEventTypeToString(InputEventType val,const char*def);
extern const char*InputKeyCodeToString(InputKeyCode val,const char*def);
extern const char*InputMouseButtonToString(InputMouseButton val,const char*def);
#endif
