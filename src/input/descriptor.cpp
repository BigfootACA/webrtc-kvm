/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include<stdint.h>
#include"hid.h"

static const uint8_t d_hid_desc_keyboard[]={
	0x05, 0x01, // Usage Page (Generic Desktop)
	0x09, 0x06, // Usage (Keyboard)
	0xa1, 0x01, // Collection (Application)
	0x05, 0x07, //   Usage Page (Key Codes)
	0x19, 0xe0, //   Usage Minimum (Keyboard LeftControl)
	0x29, 0xe7, //   Usage Maximum (Keyboard Right GUI)
	0x15, 0x00, //   Logical Minimum (0)
	0x25, 0x01, //   Logical Maximum (1)
	0x75, 0x01, //   Report Size (1)
	0x95, 0x08, //   Report Count (8)
	0x81, 0x02, //   Input (Data, Variable, Absolute)
	0x95, 0x01, //   Report Count (1)
	0x75, 0x08, //   Report Size (8)
	0x81, 0x01, //   Input (Constant)
	0x95, 0x05, //   Report Count (5)
	0x75, 0x01, //   Report Size (1)
	0x05, 0x08, //   Usage Page (LEDs)
	0x19, 0x01, //   Usage Minimum (Num Lock)
	0x29, 0x05, //   Usage Maximum (Kana)
	0x91, 0x02, //   Output (Data, Variable, Absolute)
	0x95, 0x01, //   Report Count (1)
	0x75, 0x03, //   Report Size (3)
	0x91, 0x01, //   Output (Constant)
	0x95, 0x06, //   Report Count (6)
	0x75, 0x08, //   Report Size (8)
	0x15, 0x00, //   Logical Minimum (0)
	0x25, 0x65, //   Logical Maximum (101)
	0x05, 0x07, //   Usage Page (Key Codes)
	0x19, 0x00, //   Usage Minimum (Reserved (no event indicated))
	0x29, 0x65, //   Usage Maximum (Keyboard Application)
	0x81, 0x00, //   Input (Data, Array)
	0xc0        // End Collection
};

static const uint8_t d_hid_desc_absolute[]={
	0x05, 0x01,       // Usage Page (Generic Desktop)
	0x09, 0x02,       // Usage (Mouse)
	0xa1, 0x01,       // Collection (Application)
	0x09, 0x01,       //   Usage (Pointer)
	0xa1, 0x00,       //   Collection (Physical)
	0x05, 0x09,       //     Usage Page (Button)
	0x19, 0x01,       //     Usage Minimum (Button 1)
	0x29, 0x03,       //     Usage Maximum (Button 3)
	0x15, 0x00,       //     Logical Minimum (0)
	0x25, 0x01,       //     Logical Maximum (1)
	0x95, 0x03,       //     Report Count (3)
	0x75, 0x01,       //     Report Size (1)
	0x81, 0x02,       //     Input (Data, Variable, Absolute)
	0x95, 0x01,       //     Report Count (1)
	0x75, 0x05,       //     Report Size (5)
	0x81, 0x01,       //     Input (Constant)
	0x05, 0x01,       //     Usage Page (Generic Desktop)
	0x09, 0x30,       //     Usage (X)
	0x09, 0x31,       //     Usage (Y)
	0x35, 0x00,       //     Physical Minimum (0)
	0x46, 0xff, 0x7f, //     Physical Maximum (32767)
	0x15, 0x00,       //     Logical Minimum (0)
	0x26, 0xff, 0x7f, //     Logical Maximum (32767)
	0x65, 0x11,       //     Unit (SI Lin:Distance)
	0x55, 0x00,       //     Unit Exponent (0)
	0x75, 0x10,       //     Report Size (16)
	0x95, 0x02,       //     Report Count (2)
	0x81, 0x02,       //     Input (Data, Variable, Absolute)
	0x09, 0x38,       //     Usage (Wheel)
	0x15, 0xff,       //     Logical Minimum (-1)
	0x25, 0x01,       //     Logical Maximum (1)
	0x35, 0x00,       //     Physical Minimum (-127)
	0x45, 0x00,       //     Physical Maximum (127)
	0x75, 0x08,       //     Report Size (8)
	0x95, 0x01,       //     Report Count (1)
	0x81, 0x06,       //     Input (Data, Variable, Relative)
	0xc0,             //   End Collection
	0xc0              // End Collection
};

static const uint8_t d_hid_desc_relative[]={
	0x05, 0x01,       // Usage Page (Generic Desktop)
	0x09, 0x02,       // Usage (Mouse)
	0xa1, 0x01,       // Collection (Application)
	0x09, 0x01,       //   Usage (Pointer)
	0xa1, 0x00,       //   Collection (Physical)
	0x05, 0x09,       //     Usage Page (Button)
	0x19, 0x01,       //     Usage Minimum (Button 1)
	0x29, 0x03,       //     Usage Maximum (Button 3)
	0x15, 0x00,       //     Logical Minimum (0)
	0x25, 0x01,       //     Logical Maximum (1)
	0x95, 0x03,       //     Report Count (3)
	0x75, 0x01,       //     Report Size (1)
	0x81, 0x02,       //     Input (Data, Variable, Absolute)
	0x95, 0x01,       //     Report Count (1)
	0x75, 0x05,       //     Report Size (5)
	0x81, 0x01,       //     Input (Constant)
	0x05, 0x01,       //     Usage Page (Generic Desktop)
	0x09, 0x30,       //     Usage (X)
	0x09, 0x31,       //     Usage (Y)
	0x15, 0x81,       //     Logical Minimum (-127)
	0x25, 0x7f,       //     Logical Maximum (127)
	0x75, 0x08,       //     Report Size (8)
	0x95, 0x02,       //     Report Count (2)
	0x81, 0x06,       //     Input (Data, Variable, Relative)
	0x09, 0x38,       //     Usage (Wheel)
	0x15, 0xff,       //     Logical Minimum (-1)
	0x25, 0x01,       //     Logical Maximum (1)
	0x35, 0x00,       //     Physical Minimum (-127)
	0x45, 0x00,       //     Physical Maximum (127)
	0x75, 0x08,       //     Report Size (8)
	0x95, 0x01,       //     Report Count (1)
	0x81, 0x06,       //     Input (Data, Variable, Relative)
	0xc0,             //   End Collection
	0xc0              // End Collection
};

const HIDReportDesc HIDDescKeyboard={
	.data=d_hid_desc_keyboard,
	.size=sizeof(d_hid_desc_keyboard),
};

const HIDReportDesc HIDDescAbsolute={
	.data=d_hid_desc_absolute,
	.size=sizeof(d_hid_desc_absolute),
};

const HIDReportDesc HIDDescRelative={
	.data=d_hid_desc_relative,
	.size=sizeof(d_hid_desc_relative),
};
