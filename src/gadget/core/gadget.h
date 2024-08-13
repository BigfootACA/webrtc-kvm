/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef CORE_GADGET_H
#define CORE_GADGET_H
#include"internal.h"

enum GadgetSpeed{
	USB_UNKNOWN,
	USB_SUPER_SPEED_PLUS,
	USB_SUPER_SPEED,
	USB_HIGH_SPEED,
	USB_FULL_SPEED,
	USB_LOW_SPEED,
};

class Gadget:public std::enable_shared_from_this<Gadget>,public GadgetBase{
	public:
		explicit Gadget(const std::string&name);
		virtual ~Gadget();
		DECL_ATTR(DeviceClass,uint8_t);
		DECL_ATTR(DeviceProtocol,uint8_t);
		DECL_ATTR(DeviceSubClass,uint8_t);
		DECL_ATTR(MaxPacketSize,uint8_t);
		DECL_ATTR(BcdUSB,uint16_t);
		DECL_ATTR(BcdDevice,uint16_t);
		DECL_ATTR(ProductID,uint16_t);
		DECL_ATTR(VendorID,uint16_t);
		DECL_ATTR(WebUSBVendorCode,uint8_t);
		DECL_ATTR(WebUSBBcdVersion,uint16_t);
		DECL_ATTR(WebUSBLandingPage,std::string);
		DECL_ATTR(WebUSBUse,bool);
		DECL_ATTR(OSDescVendorCode,uint8_t);
		DECL_ATTR(OSDescQWSign,std::string);
		DECL_ATTR(OSDescUse,bool);
		DECL_ATTR(UDC,std::string);
		DECL_ATTR(MaxSpeedString,std::string);
		[[nodiscard]] GadgetSpeed GetMaxSpeed()const;
		void SetMaxSpeed(GadgetSpeed val);
		inline void ResetUDC(){if(!GetUDC().empty())SetUDC("");}
		inline void PauseUDC(){last_udc=GetUDC();if(!last_udc.empty())SetUDC("");}
		inline void ResumeUDC(){if(!last_udc.empty())SetUDC(last_udc);}
		[[nodiscard]] std::string GetManufacturer(uint16_t lang);
		[[nodiscard]] std::string GetProduct(uint16_t lang);
		[[nodiscard]] std::string GetSerialNumber(uint16_t lang);
		std::shared_ptr<GadgetConfig>AddConfig(
			const std::string&id,
			uint16_t num
		);
		std::shared_ptr<GadgetString>AddString(
			uint16_t lang,
			const std::string&manufacturer="",
			const std::string&product="",
			const std::string&serialnumber=""
		);
		[[nodiscard]] std::vector<std::shared_ptr<GadgetConfig>>ListConfigs()const;
		[[nodiscard]] std::vector<std::shared_ptr<GadgetString>>ListStrings()const;
		[[nodiscard]] std::vector<std::shared_ptr<GadgetFunction>>ListFunctions()const;
		[[nodiscard]] std::string GetPath()const override;
		[[nodiscard]] inline std::string GetName()const override{return name;}
		[[nodiscard]] inline std::string GetType()const override{return "gadget";}
		void Remove()override;
		bool AutoSetUDC();
		static std::string DetectUDC();
	private:
		const std::string name;
		std::string last_udc;
};

class ClaimUDC{
	public:
		inline explicit ClaimUDC(const std::shared_ptr<Gadget>&gadget):gadget(gadget){gadget->PauseUDC();}
		inline ~ClaimUDC(){gadget->ResumeUDC();}
	private:
		std::shared_ptr<Gadget>gadget;
};

#endif
