/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"input.h"

const char*InputEventTypeToString(InputEventType val,const char*def){
	switch(val){
		case EVENT_KEY_DOWN:return "key down";
		case EVENT_KEY_UP:return "key up";
		case EVENT_MOUSE_WHEEL:return "mouse wheel";
		case EVENT_RELATIVE_DOWN:return "relative mouse down";
		case EVENT_RELATIVE_UP:return "relative mouse up";
		case EVENT_RELATIVE_MOVE:return "relative mouse move";
		case EVENT_ABSOLUTE_DOWN:return "absolute mouse down";
		case EVENT_ABSOLUTE_UP:return "absolute mouse up";
		case EVENT_ABSOLUTE_MOVE:return "absolute mouse move";
		case EVENT_TOUCH_START:return "touch start";
		case EVENT_TOUCH_MOVE:return "touch move";
		case EVENT_TOUCH_END:return "touch end";
		case EVENT_TOUCH_CANCEL:return "touch cancel";
		default:return def;
	}
}

const char*InputMouseButtonToString(InputMouseButton val,const char*def){
	switch(val){
		case BTN_MOUSE_LEFT:return "left";
		case BTN_MOUSE_WHEEL:return "wheel";
		case BTN_MOUSE_RIGHT:return "right";
		default:return def;
	}
}

const char*InputKeyCodeToString(InputKeyCode val,const char*def){
	switch(val){
		case KEY_CODE_RESERVED:return "reserved";
		case KEY_CODE_ESC:return "esc";
		case KEY_CODE_1:return "1";
		case KEY_CODE_2:return "2";
		case KEY_CODE_3:return "3";
		case KEY_CODE_4:return "4";
		case KEY_CODE_5:return "5";
		case KEY_CODE_6:return "6";
		case KEY_CODE_7:return "7";
		case KEY_CODE_8:return "8";
		case KEY_CODE_9:return "9";
		case KEY_CODE_0:return "0";
		case KEY_CODE_MINUS:return "-";
		case KEY_CODE_EQUAL:return "+";
		case KEY_CODE_BACKSPACE:return "backspace";
		case KEY_CODE_TAB:return "tab";
		case KEY_CODE_Q:return "q";
		case KEY_CODE_W:return "w";
		case KEY_CODE_E:return "e";
		case KEY_CODE_R:return "r";
		case KEY_CODE_T:return "t";
		case KEY_CODE_Y:return "y";
		case KEY_CODE_U:return "u";
		case KEY_CODE_I:return "i";
		case KEY_CODE_O:return "o";
		case KEY_CODE_P:return "p";
		case KEY_CODE_LEFTBRACE:return "{";
		case KEY_CODE_RIGHTBRACE:return "}";
		case KEY_CODE_ENTER:return "enter";
		case KEY_CODE_LEFTCTRL:return "left ctrl";
		case KEY_CODE_A:return "a";
		case KEY_CODE_S:return "s";
		case KEY_CODE_D:return "d";
		case KEY_CODE_F:return "f";
		case KEY_CODE_G:return "g";
		case KEY_CODE_H:return "h";
		case KEY_CODE_J:return "j";
		case KEY_CODE_K:return "k";
		case KEY_CODE_L:return "l";
		case KEY_CODE_SEMICOLON:return ";";
		case KEY_CODE_APOSTROPHE:return "'";
		case KEY_CODE_GRAVE:return "`";
		case KEY_CODE_LEFTSHIFT:return "left shift";
		case KEY_CODE_BACKSLASH:return "\\";
		case KEY_CODE_Z:return "z";
		case KEY_CODE_X:return "x";
		case KEY_CODE_C:return "c";
		case KEY_CODE_V:return "v";
		case KEY_CODE_B:return "b";
		case KEY_CODE_N:return "n";
		case KEY_CODE_M:return "m";
		case KEY_CODE_COMMA:return ",";
		case KEY_CODE_DOT:return ".";
		case KEY_CODE_SLASH:return "/";
		case KEY_CODE_RIGHTSHIFT:return "right shift";
		case KEY_CODE_KPASTERISK:return "keypad asterisk";
		case KEY_CODE_LEFTALT:return "left alt";
		case KEY_CODE_SPACE:return "space";
		case KEY_CODE_CAPSLOCK:return "caps lock";
		case KEY_CODE_F1:return "f1";
		case KEY_CODE_F2:return "f2";
		case KEY_CODE_F3:return "f3";
		case KEY_CODE_F4:return "f4";
		case KEY_CODE_F5:return "f5";
		case KEY_CODE_F6:return "f6";
		case KEY_CODE_F7:return "f7";
		case KEY_CODE_F8:return "f8";
		case KEY_CODE_F9:return "f9";
		case KEY_CODE_F10:return "f10";
		case KEY_CODE_NUMLOCK:return "num lock";
		case KEY_CODE_SCROLLLOCK:return "scroll lock";
		case KEY_CODE_KP7:return "keypad 7";
		case KEY_CODE_KP8:return "keypad 8";
		case KEY_CODE_KP9:return "keypad 9";
		case KEY_CODE_KPMINUS:return "keypad -";
		case KEY_CODE_KP4:return "keypad 4";
		case KEY_CODE_KP5:return "keypad 5";
		case KEY_CODE_KP6:return "keypad 6";
		case KEY_CODE_KPPLUS:return "keypad +";
		case KEY_CODE_KP1:return "keypad 1";
		case KEY_CODE_KP2:return "keypad 2";
		case KEY_CODE_KP3:return "keypad 3";
		case KEY_CODE_KP0:return "keypad 0";
		case KEY_CODE_KPDOT:return "keypad .";
		case KEY_CODE_PRTSC:return "print screen";
		case KEY_CODE_F11:return "f11";
		case KEY_CODE_F12:return "f12";
		case KEY_CODE_RO:return "ro";
		case KEY_CODE_KPENTER:return "keypad enter";
		case KEY_CODE_RIGHTCTRL:return "right ctrl";
		case KEY_CODE_KPSLASH:return "keypad /";
		case KEY_CODE_SYSRQ:return "sysrq";
		case KEY_CODE_RIGHTALT:return "right alt";
		case KEY_CODE_LINEFEED:return "line feed";
		case KEY_CODE_HOME:return "home";
		case KEY_CODE_UP:return "up";
		case KEY_CODE_PAGEUP:return "page up";
		case KEY_CODE_LEFT:return "left";
		case KEY_CODE_RIGHT:return "right";
		case KEY_CODE_END:return "end";
		case KEY_CODE_DOWN:return "down";
		case KEY_CODE_PAGEDOWN:return "page down";
		case KEY_CODE_INSERT:return "insert";
		case KEY_CODE_DELETE:return "delete";
		case KEY_CODE_MACRO:return "macro";
		case KEY_CODE_MUTE:return "mute";
		case KEY_CODE_VOLUMEDOWN:return "volume down";
		case KEY_CODE_VOLUMEUP:return "volume up";
		case KEY_CODE_POWER:return "power";
		case KEY_CODE_KPEQUAL:return "keypad equal";
		case KEY_CODE_KPPLUSMINUS:return "keypad plus minus";
		case KEY_CODE_PAUSE:return "pause";
		case KEY_CODE_SCALE:return "scale";
		case KEY_CODE_KPCOMMA:return "keypad ,";
		case KEY_CODE_LEFTMETA:return "left meta";
		case KEY_CODE_RIGHTMETA:return "right meta";
		case KEY_CODE_COMPOSE:return "compose";
		case KEY_CODE_STOP:return "stop";
		case KEY_CODE_AGAIN:return "again";
		case KEY_CODE_PROPS:return "props";
		case KEY_CODE_UNDO:return "undo";
		case KEY_CODE_FRONT:return "front";
		case KEY_CODE_COPY:return "copy";
		case KEY_CODE_OPEN:return "open";
		case KEY_CODE_PASTE:return "paste";
		case KEY_CODE_FIND:return "find";
		case KEY_CODE_CUT:return "cut";
		case KEY_CODE_HELP:return "help";
		case KEY_CODE_MENU:return "menu";
		case KEY_CODE_CALC:return "calc";
		case KEY_CODE_SETUP:return "setup";
		case KEY_CODE_SLEEP:return "sleep";
		case KEY_CODE_WAKEUP:return "wakeup";
		case KEY_CODE_FILE:return "file";
		case KEY_CODE_SENDFILE:return "send file";
		case KEY_CODE_DELETEFILE:return "delete file";
		case KEY_CODE_XFER:return "xfer";
		case KEY_CODE_PROG1:return "prog1";
		case KEY_CODE_PROG2:return "prog2";
		case KEY_CODE_WWW:return "www";
		case KEY_CODE_MSDOS:return "msdos";
		case KEY_CODE_SCREENLOCK:return "screen lock";
		case KEY_CODE_DIRECTION:return "direction";
		case KEY_CODE_CYCLEWINDOWS:return "cycle windows";
		case KEY_CODE_MAIL:return "mail";
		case KEY_CODE_BOOKMARKS:return "bookmarks";
		case KEY_CODE_COMPUTER:return "computer";
		case KEY_CODE_BACK:return "back";
		case KEY_CODE_FORWARD:return "forward";
		case KEY_CODE_CLOSECD:return "closecd";
		case KEY_CODE_EJECTCD:return "ejectcd";
		case KEY_CODE_EJECTCLOSECD:return "eject closecd";
		case KEY_CODE_NEXTSONG:return "next song";
		case KEY_CODE_PLAYPAUSE:return "play pause";
		case KEY_CODE_PREVIOUSSONG:return "previous song";
		case KEY_CODE_STOPCD:return "stop cd";
		case KEY_CODE_RECORD:return "record";
		case KEY_CODE_REWIND:return "rewind";
		case KEY_CODE_PHONE:return "phone";
		case KEY_CODE_ISO:return "iso";
		case KEY_CODE_CONFIG:return "config";
		case KEY_CODE_HOMEPAGE:return "home page";
		case KEY_CODE_REFRESH:return "refresh";
		case KEY_CODE_EXIT:return "exit";
		case KEY_CODE_MOVE:return "move";
		case KEY_CODE_EDIT:return "edit";
		case KEY_CODE_SCROLLUP:return "scroll up";
		case KEY_CODE_SCROLLDOWN:return "scroll down";
		case KEY_CODE_KPLEFTPAREN:return "keypad left paren";
		case KEY_CODE_KPRIGHTPAREN:return "keypad right paren";
		case KEY_CODE_NEW:return "new";
		case KEY_CODE_REDO:return "redo";
		case KEY_CODE_F13:return "f13";
		case KEY_CODE_F14:return "f14";
		case KEY_CODE_F15:return "f15";
		case KEY_CODE_F16:return "f16";
		case KEY_CODE_F17:return "f17";
		case KEY_CODE_F18:return "f18";
		case KEY_CODE_F19:return "f19";
		case KEY_CODE_F20:return "f20";
		case KEY_CODE_F21:return "f21";
		case KEY_CODE_F22:return "f22";
		case KEY_CODE_F23:return "f23";
		case KEY_CODE_F24:return "f24";
		case KEY_CODE_PLAYCD:return "play cd";
		case KEY_CODE_PAUSECD:return "pause cd";
		case KEY_CODE_PROG3:return "prog3";
		case KEY_CODE_PROG4:return "prog4";
		case KEY_CODE_DASHBOARD:return "dashboard";
		case KEY_CODE_SUSPEND:return "suspend";
		case KEY_CODE_CLOSE:return "close";
		case KEY_CODE_PLAY:return "play";
		case KEY_CODE_FASTFORWARD:return "fast forward";
		case KEY_CODE_BASSBOOST:return "bass boost";
		case KEY_CODE_PRINT:return "print";
		case KEY_CODE_HP:return "hp";
		case KEY_CODE_CAMERA:return "camera";
		case KEY_CODE_SOUND:return "sound";
		case KEY_CODE_QUESTION:return "question";
		case KEY_CODE_EMAIL:return "email";
		case KEY_CODE_CHAT:return "chat";
		case KEY_CODE_SEARCH:return "search";
		case KEY_CODE_CONNECT:return "connect";
		case KEY_CODE_FINANCE:return "finance";
		case KEY_CODE_SPORT:return "sport";
		case KEY_CODE_SHOP:return "shop";
		case KEY_CODE_ALTERASE:return "alt erase";
		case KEY_CODE_CANCEL:return "cancel";
		case KEY_CODE_BRIGHTNESSDOWN:return "brightness down";
		case KEY_CODE_BRIGHTNESSUP:return "brightness up";
		case KEY_CODE_MEDIA:return "media";
		case KEY_CODE_SWITCHVIDEOMODE:return "switch video mode";
		case KEY_CODE_KBDILLUMTOGGLE:return "kbd illum toggle";
		case KEY_CODE_KBDILLUMDOWN:return "kbdillum down";
		case KEY_CODE_KBDILLUMUP:return "kbd illum up";
		case KEY_CODE_SEND:return "send";
		case KEY_CODE_REPLY:return "reply";
		case KEY_CODE_FORWARDMAIL:return "forward mail";
		case KEY_CODE_SAVE:return "save";
		case KEY_CODE_DOCUMENTS:return "documents";
		case KEY_CODE_BATTERY:return "battery";
		case KEY_CODE_BLUETOOTH:return "bluetooth";
		case KEY_CODE_WLAN:return "wlan";
		case KEY_CODE_UWB:return "uwb";
		case KEY_CODE_UNKNOWN:return "unknown";
		case KEY_CODE_VIDEO_NEXT:return "video next";
		case KEY_CODE_VIDEO_PREV:return "video prev";
		case KEY_CODE_BRIGHTNESS_CYCLE:return "brightness cycle";
		case KEY_CODE_BRIGHTNESS_AUTO:return "brightness auto";
		case KEY_CODE_DISPLAY_OFF:return "display off";
		case KEY_CODE_WWAN:return "wwan";
		case KEY_CODE_RFKILL:return "rfkill";
		case KEY_CODE_MICMUTE:return "mic mute";
		default:return def;
	}
}
