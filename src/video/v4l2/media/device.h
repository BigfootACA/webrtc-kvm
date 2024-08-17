/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef V4L2_MEDIA_DEVICE_H
#define V4L2_MEDIA_DEVICE_H
#include<string>
#include<memory>
#include<vector>
#include<linux/media.h>

class MediaTopology;
class MediaEntityDesc;

class MediaDevice:public std::enable_shared_from_this<MediaDevice>{
	friend class MediaEntityDesc;
	friend class MediaTopology;
	public:
		~MediaDevice();
		explicit MediaDevice(dev_t dev);
		explicit MediaDevice(const std::string&path);
		explicit MediaDevice(int fd,bool auto_close=false);
		[[nodiscard]] std::shared_ptr<MediaTopology>GetTopology();
		[[nodiscard]] std::vector<std::shared_ptr<MediaEntityDesc>>EnumEntities();
		[[nodiscard]] std::shared_ptr<MediaEntityDesc>EnumEntity(uint32_t id);
		[[nodiscard]] std::string GetDriver()const;
		[[nodiscard]] std::string GetModel()const;
		[[nodiscard]] std::string GetSerial()const;
		[[nodiscard]] std::string GetBusInfo()const;
		[[nodiscard]] inline uint32_t GetMediaVersion()const{return info.media_version;}
		[[nodiscard]] inline uint32_t GetHardwareRevision()const{return info.hw_revision;}
		[[nodiscard]] inline uint32_t GetDriverVersion()const{return info.driver_version;}
		[[nodiscard]] static std::shared_ptr<MediaDevice>FromVideoDevice(dev_t vd);
		[[nodiscard]] static std::shared_ptr<MediaDevice>FromVideoFD(int fd);
		[[nodiscard]] std::string ToString()const;
		[[nodiscard]] inline explicit operator std::string()const{return ToString();}
	protected:
		[[nodiscard]] static dev_t MediaFromVideo(dev_t dev);
		void Initialize();
		int fd=-1;
		bool auto_close=false;
		media_device_info info{};
};

#endif
