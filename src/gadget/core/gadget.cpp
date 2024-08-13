/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"internal.h"
#include"gadget.h"
#include"gadget_config.h"
#include"gadget_string.h"
#include"gadget_function.h"
#include"lib/exception.h"
#include"lib/modules.h"

Gadget::Gadget(const std::string&name):name(name){
	if(name.empty()||!CheckValid(name,VALIDS))
		throw RuntimeError("bad name {}",name);
	LoadModule("libcomposite");
	Create();
}

Gadget::~Gadget(){

}

std::string Gadget::DetectUDC(){
	std::string selected;
	FolderReader d("/sys/class/udc/");
	std::shared_ptr<FolderReaderItem>item;
	while((item=d.ReadItem())){
		if(item->IsVirtual()||!item->IsSymlink())continue;
		if(
			!selected.empty()&&
			selected.find("usbip-vudc")==std::string::npos&&
			selected.find("dummy")==std::string::npos
		)continue;
		selected=item->GetName();
	}
	return selected;
}

bool Gadget::AutoSetUDC(){
	auto udc=DetectUDC();
	if(udc.empty())return false;
	SetUDC(udc);
	return true;
}

GadgetSpeed Gadget::GetMaxSpeed()const{
	auto val=GetMaxSpeedString();
	if(val=="super-speed-plus")return USB_SUPER_SPEED_PLUS;
	if(val=="super-speed")return USB_SUPER_SPEED;
	if(val=="high-speed")return USB_HIGH_SPEED;
	if(val=="full-speed")return USB_FULL_SPEED;
	if(val=="low-speed")return USB_LOW_SPEED;
	return USB_UNKNOWN;
}

void Gadget::SetMaxSpeed(GadgetSpeed val){
	std::string str;
	switch(val){
		case USB_SUPER_SPEED_PLUS:str="super-speed-plus";break;
		case USB_SUPER_SPEED:str="super-speed";break;
		case USB_HIGH_SPEED:str="high-speed";break;
		case USB_FULL_SPEED:str="full-speed";break;
		case USB_LOW_SPEED:str="low-speed";break;
		default:return;
	}
	SetMaxSpeedString(str);
}

void Gadget::Remove(){
	ResetUDC();
	for(const auto&s:ListConfigs())s->Remove();
	for(const auto&s:ListFunctions())s->Remove();
	for(const auto&s:ListStrings())s->Remove();
	RemoveFolder(GetPath());
}

std::vector<std::shared_ptr<GadgetConfig>>Gadget::ListConfigs()const{
	return ListSubItems<Gadget,GadgetConfig>(this,"configs");
}

std::vector<std::shared_ptr<GadgetString>>Gadget::ListStrings()const{
	return ListSubItems<Gadget,GadgetString>(this,"strings");
}

std::vector<std::shared_ptr<GadgetFunction>>Gadget::ListFunctions()const{
	std::vector<std::shared_ptr<GadgetFunction>>res;
	FolderReader d(std::format("{}/functions",GetPath()));
	std::shared_ptr<FolderReaderItem>item;
	auto shared=shared_from_this();
	while((item=d.ReadItem())){
		if(item->IsHidden()||!item->IsFolder())continue;
		auto val=GadgetFunction::FromName(shared,item->GetName());
		if(val)res.push_back(val);
	}
	return res;
}

std::string Gadget::GetPath()const{
	return std::format("/sys/kernel/config/usb_gadget/{}",GetName());
}

std::shared_ptr<GadgetString>Gadget::AddString(
	const uint16_t lang,
	const std::string&manufacturer,
	const std::string&product,
	const std::string&serialnumber
){
	auto str=std::make_shared<GadgetString>(shared_from_this(),lang);
	if(!str)return nullptr;
	if(!manufacturer.empty())str->SetManufacturer(manufacturer);
	if(!product.empty())str->SetProduct(product);
	if(!serialnumber.empty())str->SetSerialNumber(serialnumber);
	return str;
}

std::shared_ptr<GadgetConfig> Gadget::AddConfig(const std::string&id,uint16_t num){
	return std::make_shared<GadgetConfig>(shared_from_this(),id,num);
}

std::string Gadget::GetManufacturer(const uint16_t lang){
	return AddString(lang)->GetManufacturer();
}

std::string Gadget::GetProduct(const uint16_t lang){
	return AddString(lang)->GetProduct();
}

std::string Gadget::GetSerialNumber(const uint16_t lang){
	return AddString(lang)->GetSerialNumber();
}

IMPL_ATTR_HEX8(Gadget,DeviceClass,bDeviceClass)
IMPL_ATTR_HEX8(Gadget,DeviceProtocol,bDeviceProtocol)
IMPL_ATTR_HEX8(Gadget,DeviceSubClass,bDeviceSubClass)
IMPL_ATTR_HEX8(Gadget,MaxPacketSize,bMaxPacketSize0)
IMPL_ATTR_HEX16(Gadget,BcdUSB,bcdUSB)
IMPL_ATTR_HEX16(Gadget,BcdDevice,bcdDevice)
IMPL_ATTR_HEX16(Gadget,ProductID,idProduct)
IMPL_ATTR_HEX16(Gadget,VendorID,idVendor)
IMPL_ATTR_HEX8(Gadget,WebUSBVendorCode,webusb/bVendorCode)
IMPL_ATTR_HEX16(Gadget,WebUSBBcdVersion,webusb/bcdVersion)
IMPL_ATTR_STRING(Gadget,WebUSBLandingPage,webusb/landingPage)
IMPL_ATTR_BOOL(Gadget,WebUSBUse,webusb/use)
IMPL_ATTR_HEX8(Gadget,OSDescVendorCode,os_desc/b_vendor_code)
IMPL_ATTR_STRING(Gadget,OSDescQWSign,os_desc/qw_sign)
IMPL_ATTR_BOOL(Gadget,OSDescUse,os_desc/use)
IMPL_ATTR_STRING(Gadget,UDC,UDC)
IMPL_ATTR_STRING(Gadget,MaxSpeedString,max_speed)
