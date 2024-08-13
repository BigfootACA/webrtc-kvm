/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef HID_H
#define HID_H
#include<cstdint>
#include<cstddef>
#include<cassert>
#include"defines.h"
#include"input.h"

enum HIDKeyCode{
	HID_KEY_UNKNOWN     = 0x00,
	HID_KEY_A           = 0x04,
	HID_KEY_B           = 0x05,
	HID_KEY_C           = 0x06,
	HID_KEY_D           = 0x07,
	HID_KEY_E           = 0x08,
	HID_KEY_F           = 0x09,
	HID_KEY_G           = 0x0a,
	HID_KEY_H           = 0x0b,
	HID_KEY_I           = 0x0c,
	HID_KEY_J           = 0x0d,
	HID_KEY_K           = 0x0e,
	HID_KEY_L           = 0x0f,
	HID_KEY_M           = 0x10,
	HID_KEY_N           = 0x11,
	HID_KEY_O           = 0x12,
	HID_KEY_P           = 0x13,
	HID_KEY_Q           = 0x14,
	HID_KEY_R           = 0x15,
	HID_KEY_S           = 0x16,
	HID_KEY_T           = 0x17,
	HID_KEY_U           = 0x18,
	HID_KEY_V           = 0x19,
	HID_KEY_W           = 0x1a,
	HID_KEY_X           = 0x1b,
	HID_KEY_Y           = 0x1c,
	HID_KEY_Z           = 0x1d,
	HID_KEY_1           = 0x1e,
	HID_KEY_2           = 0x1f,
	HID_KEY_3           = 0x20,
	HID_KEY_4           = 0x21,
	HID_KEY_5           = 0x22,
	HID_KEY_6           = 0x23,
	HID_KEY_7           = 0x24,
	HID_KEY_8           = 0x25,
	HID_KEY_9           = 0x26,
	HID_KEY_0           = 0x27,
	HID_KEY_RETURN      = 0x28,
	HID_KEY_ESC         = 0x29,
	HID_KEY_BACKSPACE   = 0x2a,
	HID_KEY_TAB         = 0x2b,
	HID_KEY_SPACE       = 0x2c,
	HID_KEY_MINUS       = 0x2d,
	HID_KEY_EQUAL       = 0x2e,
	HID_KEY_LEFTBRACE   = 0x2f,
	HID_KEY_RIGHTBRACE  = 0x30,
	HID_KEY_BACKSLASH   = 0x31,
	HID_KEY_HASH        = 0x32,
	HID_KEY_SEMICOLON   = 0x33,
	HID_KEY_APOSTROPHE  = 0x34,
	HID_KEY_GRAVE       = 0x35,
	HID_KEY_COMMA       = 0x36,
	HID_KEY_DOT         = 0x37,
	HID_KEY_SLASH       = 0x38,
	HID_KEY_CAPSLOCK    = 0x39,
	HID_KEY_F1          = 0x3a,
	HID_KEY_F2          = 0x3b,
	HID_KEY_F3          = 0x3c,
	HID_KEY_F4          = 0x3d,
	HID_KEY_F5          = 0x3e,
	HID_KEY_F6          = 0x3f,
	HID_KEY_F7          = 0x40,
	HID_KEY_F8          = 0x41,
	HID_KEY_F9          = 0x42,
	HID_KEY_F10         = 0x43,
	HID_KEY_F11         = 0x44,
	HID_KEY_F12         = 0x45,
	HID_KEY_PRINT       = 0x46,
	HID_KEY_SCROLLLOCK  = 0x47,
	HID_KEY_PAUSE       = 0x48,
	HID_KEY_INSERT      = 0x49,
	HID_KEY_HOME        = 0x4a,
	HID_KEY_PAGEUP      = 0x4b,
	HID_KEY_DELETE      = 0x4c,
	HID_KEY_END         = 0x4d,
	HID_KEY_PAGEDOWN    = 0x4e,
	HID_KEY_RIGHT       = 0x4f,
	HID_KEY_LEFT        = 0x50,
	HID_KEY_DOWN        = 0x51,
	HID_KEY_UP          = 0x52,
	HID_KEY_NUMLOCK     = 0x53,
	HID_KEY_KP_DIVIDE   = 0x54,
	HID_KEY_KP_MULTIPLY = 0x55,
	HID_KEY_KP_SUBTRACT = 0x56,
	HID_KEY_KP_ADD      = 0x57,
	HID_KEY_KP_ENTER    = 0x58,
	HID_KEY_KP_1        = 0x59,
	HID_KEY_KP_2        = 0x5a,
	HID_KEY_KP_3        = 0x5b,
	HID_KEY_KP_4        = 0x5c,
	HID_KEY_KP_5        = 0x5d,
	HID_KEY_KP_6        = 0x5e,
	HID_KEY_KP_7        = 0x5f,
	HID_KEY_KP_8        = 0x60,
	HID_KEY_KP_9        = 0x61,
	HID_KEY_KP_0        = 0x62,
	HID_KEY_KP_DECIMAL  = 0x63,
	HID_KEY_APPLICATION = 0x65,
	HID_KEY_POWER       = 0x66,
	HID_KEY_KP_EQUAL    = 0x67,
	HID_KEY_F13         = 0x68,
	HID_KEY_F14         = 0x69,
	HID_KEY_F15         = 0x6a,
	HID_KEY_F16         = 0x6b,
	HID_KEY_F17         = 0x6c,
	HID_KEY_F18         = 0x6d,
	HID_KEY_F19         = 0x6e,
	HID_KEY_F20         = 0x6f,
	HID_KEY_F21         = 0x70,
	HID_KEY_F22         = 0x71,
	HID_KEY_F23         = 0x72,
	HID_KEY_F24         = 0x73,
	HID_KEY_KP_EXECUTE  = 0x74,
}cdecl_attr_packed;

enum HIDKeyMod{
	HID_MOD_UNKNOWN     = 0x00,
	HID_MOD_CTRL_LEFT   = 0x01,
	HID_MOD_CTRL_RIGHT  = 0x10,
	HID_MOD_SHIFT_LEFT  = 0x02,
	HID_MOD_SHIFT_RIGHT = 0x20,
	HID_MOD_ALT_LEFT    = 0x04,
	HID_MOD_ALT_RIGHT   = 0x40,
	HID_MOD_META_LEFT   = 0x08,
	HID_MOD_META_RIGHT  = 0x80,
}cdecl_attr_packed;

enum HIDMouseBtn{
	HID_BTN_MOUSE_LEFT   = 0x01,
	HID_BTN_MOUSE_RIGHT  = 0x02,
	HID_BTN_MOUSE_MIDDLE = 0x04,
}cdecl_attr_packed;

struct HIDReportDesc{
	const uint8_t*data;
	const size_t size;
};

struct HIDKeyboardReport{
	HIDKeyMod mod:8;
	uint8_t:8;
	HIDKeyCode key[6];
}cdecl_attr_packed;

struct HIDRelativeReport{
	HIDMouseBtn btn;
	int8_t delta_x;
	int8_t delta_y;
	int8_t wheel;
}cdecl_attr_packed;

struct HIDAbsoluteReport{
	HIDMouseBtn btn;
	uint16_t position_x;
	uint16_t position_y;
	int8_t wheel;
}cdecl_attr_packed;

struct HIDTouchscreenReport{
}cdecl_attr_packed;

static_assert(sizeof(HIDKeyboardReport)==8,"HID Keyboard report size mismatch");
static_assert(sizeof(HIDRelativeReport)==4,"HID Relative Mouse report size mismatch");
static_assert(sizeof(HIDAbsoluteReport)==6,"HID Absolute Mouse report size mismatch");

extern const HIDReportDesc HIDDescKeyboard;
extern const HIDReportDesc HIDDescAbsolute;
extern const HIDReportDesc HIDDescRelative;
extern HIDKeyCode InputKeyToHIDCode(InputKeyCode code);
extern HIDKeyMod InputKeyToHIDMod(InputKeyCode code);
extern void InputFillHIDKeyboard(HIDKeyboardReport*r,const InputEvent*ev);
extern void InputFillHIDRelative(HIDRelativeReport*r,const InputEvent*ev);
extern void InputFillHIDAbsolute(HIDAbsoluteReport*r,const InputEvent*ev);
extern void InputFillHIDTouchscreen(HIDTouchscreenReport*r,const InputEvent*ev);

#endif
