/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"hid.h"
#include"input.h"

HIDKeyCode InputKeyToHIDCode(InputKeyCode code){
	switch(code){
		case KEY_CODE_ESC:return HID_KEY_ESC;
		case KEY_CODE_1:return HID_KEY_1;
		case KEY_CODE_2:return HID_KEY_2;
		case KEY_CODE_3:return HID_KEY_3;
		case KEY_CODE_4:return HID_KEY_4;
		case KEY_CODE_5:return HID_KEY_5;
		case KEY_CODE_6:return HID_KEY_6;
		case KEY_CODE_7:return HID_KEY_7;
		case KEY_CODE_8:return HID_KEY_8;
		case KEY_CODE_9:return HID_KEY_9;
		case KEY_CODE_0:return HID_KEY_0;
		case KEY_CODE_MINUS:return HID_KEY_MINUS;
		case KEY_CODE_EQUAL:return HID_KEY_EQUAL;
		case KEY_CODE_BACKSPACE:return HID_KEY_BACKSPACE;
		case KEY_CODE_TAB:return HID_KEY_TAB;
		case KEY_CODE_Q:return HID_KEY_Q;
		case KEY_CODE_W:return HID_KEY_W;
		case KEY_CODE_E:return HID_KEY_E;
		case KEY_CODE_R:return HID_KEY_R;
		case KEY_CODE_T:return HID_KEY_T;
		case KEY_CODE_Y:return HID_KEY_Y;
		case KEY_CODE_U:return HID_KEY_U;
		case KEY_CODE_I:return HID_KEY_I;
		case KEY_CODE_O:return HID_KEY_O;
		case KEY_CODE_P:return HID_KEY_P;
		case KEY_CODE_LEFTBRACE:return HID_KEY_LEFTBRACE;
		case KEY_CODE_RIGHTBRACE:return HID_KEY_RIGHTBRACE;
		case KEY_CODE_ENTER:return HID_KEY_RETURN;
		case KEY_CODE_A:return HID_KEY_A;
		case KEY_CODE_S:return HID_KEY_S;
		case KEY_CODE_D:return HID_KEY_D;
		case KEY_CODE_F:return HID_KEY_F;
		case KEY_CODE_G:return HID_KEY_G;
		case KEY_CODE_H:return HID_KEY_H;
		case KEY_CODE_J:return HID_KEY_J;
		case KEY_CODE_K:return HID_KEY_K;
		case KEY_CODE_L:return HID_KEY_L;
		case KEY_CODE_SEMICOLON:return HID_KEY_SEMICOLON;
		case KEY_CODE_APOSTROPHE:return HID_KEY_APOSTROPHE;
		case KEY_CODE_GRAVE:return HID_KEY_GRAVE;
		case KEY_CODE_BACKSLASH:return HID_KEY_BACKSLASH;
		case KEY_CODE_Z:return HID_KEY_Z;
		case KEY_CODE_X:return HID_KEY_X;
		case KEY_CODE_C:return HID_KEY_C;
		case KEY_CODE_V:return HID_KEY_V;
		case KEY_CODE_B:return HID_KEY_B;
		case KEY_CODE_N:return HID_KEY_N;
		case KEY_CODE_M:return HID_KEY_M;
		case KEY_CODE_COMMA:return HID_KEY_COMMA;
		case KEY_CODE_DOT:return HID_KEY_DOT;
		case KEY_CODE_SLASH:return HID_KEY_SLASH;
		case KEY_CODE_SPACE:return HID_KEY_SPACE;
		case KEY_CODE_CAPSLOCK:return HID_KEY_CAPSLOCK;
		case KEY_CODE_F1:return HID_KEY_F1;
		case KEY_CODE_F2:return HID_KEY_F2;
		case KEY_CODE_F3:return HID_KEY_F3;
		case KEY_CODE_F4:return HID_KEY_F4;
		case KEY_CODE_F5:return HID_KEY_F5;
		case KEY_CODE_F6:return HID_KEY_F6;
		case KEY_CODE_F7:return HID_KEY_F7;
		case KEY_CODE_F8:return HID_KEY_F8;
		case KEY_CODE_F9:return HID_KEY_F9;
		case KEY_CODE_F10:return HID_KEY_F10;
		case KEY_CODE_NUMLOCK:return HID_KEY_NUMLOCK;
		case KEY_CODE_SCROLLLOCK:return HID_KEY_SCROLLLOCK;
		case KEY_CODE_KP7:return HID_KEY_KP_7;
		case KEY_CODE_KP8:return HID_KEY_KP_8;
		case KEY_CODE_KP9:return HID_KEY_KP_9;
		case KEY_CODE_KPMINUS:return HID_KEY_KP_SUBTRACT;
		case KEY_CODE_KP4:return HID_KEY_KP_4;
		case KEY_CODE_KP5:return HID_KEY_KP_5;
		case KEY_CODE_KP6:return HID_KEY_KP_6;
		case KEY_CODE_KPPLUS:return HID_KEY_KP_ADD;
		case KEY_CODE_KP1:return HID_KEY_KP_1;
		case KEY_CODE_KP2:return HID_KEY_KP_2;
		case KEY_CODE_KP3:return HID_KEY_KP_3;
		case KEY_CODE_KP0:return HID_KEY_KP_0;
		case KEY_CODE_KPDOT:return HID_KEY_KP_DECIMAL;
		case KEY_CODE_PRTSC:return HID_KEY_PRINT;
		case KEY_CODE_F11:return HID_KEY_F11;
		case KEY_CODE_F12:return HID_KEY_F12;
		case KEY_CODE_KPENTER:return HID_KEY_KP_ENTER;
		case KEY_CODE_KPSLASH:return HID_KEY_KP_DIVIDE;
		case KEY_CODE_HOME:return HID_KEY_HOME;
		case KEY_CODE_UP:return HID_KEY_UP;
		case KEY_CODE_PAGEUP:return HID_KEY_PAGEUP;
		case KEY_CODE_LEFT:return HID_KEY_LEFT;
		case KEY_CODE_RIGHT:return HID_KEY_RIGHT;
		case KEY_CODE_END:return HID_KEY_END;
		case KEY_CODE_DOWN:return HID_KEY_DOWN;
		case KEY_CODE_PAGEDOWN:return HID_KEY_PAGEDOWN;
		case KEY_CODE_INSERT:return HID_KEY_INSERT;
		case KEY_CODE_DELETE:return HID_KEY_DELETE;
		case KEY_CODE_POWER:return HID_KEY_POWER;
		case KEY_CODE_KPEQUAL:return HID_KEY_KP_EQUAL;
		case KEY_CODE_PAUSE:return HID_KEY_PAUSE;
		case KEY_CODE_KPCOMMA:return HID_KEY_KP_MULTIPLY;
		case KEY_CODE_F13:return HID_KEY_F13;
		case KEY_CODE_F14:return HID_KEY_F14;
		case KEY_CODE_F15:return HID_KEY_F15;
		case KEY_CODE_F16:return HID_KEY_F16;
		case KEY_CODE_F17:return HID_KEY_F17;
		case KEY_CODE_F18:return HID_KEY_F18;
		case KEY_CODE_F19:return HID_KEY_F19;
		case KEY_CODE_F20:return HID_KEY_F20;
		case KEY_CODE_F21:return HID_KEY_F21;
		case KEY_CODE_F22:return HID_KEY_F22;
		case KEY_CODE_F23:return HID_KEY_F23;
		case KEY_CODE_F24:return HID_KEY_F24;
		case KEY_CODE_PRINT:return HID_KEY_PRINT;
		default:return HID_KEY_UNKNOWN;
	}
}

HIDKeyMod InputKeyToHIDMod(InputKeyCode code){
	switch(code){
		case KEY_CODE_LEFTCTRL:return HID_MOD_CTRL_LEFT;
		case KEY_CODE_RIGHTCTRL:return HID_MOD_CTRL_RIGHT;
		case KEY_CODE_LEFTSHIFT:return HID_MOD_SHIFT_LEFT;
		case KEY_CODE_RIGHTSHIFT:return HID_MOD_SHIFT_RIGHT;
		case KEY_CODE_LEFTALT:return HID_MOD_ALT_LEFT;
		case KEY_CODE_RIGHTALT:return HID_MOD_ALT_RIGHT;
		case KEY_CODE_LEFTMETA:return HID_MOD_META_LEFT;
		case KEY_CODE_RIGHTMETA:return HID_MOD_META_RIGHT;
		default:return HID_MOD_UNKNOWN;
	}
}
