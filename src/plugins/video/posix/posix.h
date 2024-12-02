/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef POSIX_STREAM_H
#define POSIX_STREAM_H
#include<sys/un.h>
#include<sys/poll.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include"stream.h"
#include"algo/url.h"

struct SocketAddress{
	union{
		sockaddr sa;
		sockaddr_in sin;
		sockaddr_in6 sin6;
		sockaddr_storage stor;
		sockaddr_un sun;
	};
	union{
		socklen_t slen;
		size_t size;
		uint32_t len32;
		uint64_t len64;
	};
};

struct SocketClient{
	int fd;
	SocketAddress addr;
	UUID poll;
};

enum PosixMode{
	MODE_UNKNOWN,
	MODE_SOCKET,
	MODE_FILE,
	MODE_FD,
};

struct DataBuffer{
	void*buffer;
	size_t offset;
	size_t size;
};

class PosixStream:public Stream{
	protected:
		void OnLoadDeviceConfig(YAML::Node&cfg)override;
		void OnProcessInput(StreamBuffer*buffer)override;
		void ProcessEvent(EventHandlerContext*ev)override;
		void OnInitialize()override;
		void OnDeinitialize()override;
		void OnStartStream()override;
		void OnStopStream()override;
		void ProcessNewClient();
		void ProcessWriteData(int fd);
		void DetectMTU(int fd);
		void ShutdownFD(int fd,bool direct=false);
		void OpenSocket();
		void OpenFile();
		void OpenFD();
		void Parse();
		bool want_close=false;
		URL url;
		SocketAddress addr{};
		std::map<int,SocketClient>clients;
		int sock_type=0;
		int sock_proto=0;
		bool sock_listen=false;
		bool sock_accept=false;
		bool dir_read=false;
		bool dir_write=false;
		std::vector<DataBuffer>buffers;
		PosixMode mode=MODE_UNKNOWN;
		int mtu=INT_MAX;
};

class PosixOutputStream:public PosixStream{
	public:
		~PosixOutputStream()override=default;
		explicit PosixOutputStream(webrtc_kvm*ctx);
		[[nodiscard]] inline std::string GetDriverName()const final{return "posix-out";}
		[[nodiscard]] inline StreamType GetType()const override{return STREAM_SINK;}
		[[nodiscard]] inline uint32_t GetWidth()const override{return width;}
		[[nodiscard]] inline uint32_t GetHeight()const override{return height;}
		[[nodiscard]] inline uint32_t GetFrameRate()const override{return fps;}
	protected:
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
		uint32_t width=0,height=0,fps=0;
};

class PosixInputStream:public PosixStream{
	public:
		~PosixInputStream()override=default;
		explicit PosixInputStream(webrtc_kvm*ctx);
		[[nodiscard]] inline std::string GetDriverName()const final{return "posix-in";}
		[[nodiscard]] inline StreamType GetType()const override{return STREAM_SOURCE;}
		[[nodiscard]] inline uint32_t GetWidth()const override{return GetInputStream()->GetWidth();}
		[[nodiscard]] inline uint32_t GetHeight()const override{return GetInputStream()->GetHeight();}
		[[nodiscard]] inline uint32_t GetFrameRate()const override{return GetInputStream()->GetFrameRate();}
	protected:
		void OnLoadDeviceConfig(YAML::Node&cfg)final;
};

#endif
