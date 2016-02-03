
#pragma once

#ifndef __linux
#	error "system other than linux is currently not supported."
#endif

#include "meta.hpp"
#include "misc.hpp"
#include <string>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

namespace stage {

static inline bool isLanIP(in_addr_t ip_network_endian)
{
	static uint32_t lanip_masks[3];
	static bool init_flag = false;
	if (!init_flag)
	{
		struct in_addr addr;

		inet_aton("10.0.0.0", &addr);
		lanip_masks[0] = ntohl(addr.s_addr) >> 24;

		inet_aton("172.16.0.0", &addr);
		lanip_masks[1] = ntohl(addr.s_addr) >> 20;

		inet_aton("192.168.0.0", &addr);
		lanip_masks[2] = ntohl(addr.s_addr) >> 16;

		init_flag = true;
	}
	ip_network_endian = ntohl(ip_network_endian);

	return ((ip_network_endian >> 24) == lanip_masks[0])
		|| ((ip_network_endian >> 20) == lanip_masks[1])
		|| ((ip_network_endian >> 16) == lanip_masks[2]);
}

static inline bool isWanIP(in_addr_t ip)
{
	uint32_t loopback = 0;
	if (loopback == 0)
	{
		struct in_addr addr;
		inet_aton("127.0.0.1", &addr);
		loopback= addr.s_addr;
	}
	return (ip != loopback) && !isLanIP(ip);
}

typedef bool (*ipJudgeFunc) (in_addr_t addr);

static inline std::string iterOnInterface(ipJudgeFunc judge, const std::string& fallback)
{
	struct ifaddrs* ifAddrList = NULL;
	getifaddrs(&ifAddrList);

	for (struct ifaddrs* ifa = ifAddrList; ifa != NULL; ifa = ifa->ifa_next)
	{
		if ((ifa ->ifa_flags & IFF_UP) && ifa->ifa_addr->sa_family == AF_INET)
		{
			struct in_addr addr = reinterpret_cast<struct sockaddr_in*>(ifa->ifa_addr)->sin_addr;
			if (judge(addr.s_addr))
			{
				freeifaddrs(ifAddrList);
				char addressBuffer[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &addr, addressBuffer, INET_ADDRSTRLEN);
				return std::string(addressBuffer);
			}
		}
	}
	if (ifAddrList != NULL)
	{
		freeifaddrs(ifAddrList);
	}
	return fallback;
}

static inline std::string getLanIP(const std::string& fallback = std::string("127.0.0.1"))
{
	return iterOnInterface(isLanIP, fallback);
}

// NOTE: single interface only
static inline std::string getWanIP(const std::string& fallback = std::string("0.0.0.0"))
{
	return iterOnInterface(isWanIP, fallback);
}

template<typename BytesType>
inline BytesType ntoh(BytesType a)
{
	BOOST_STATIC_ASSERT(sizeof(char) == 1);
	char assist[sizeof(BytesType)];
	memcpy(assist, &a, sizeof(BytesType));
	char c;
	for (int i = 0; i < (sizeof(BytesType) >> 1); ++i)
	{
		c = assist[i];
		assist[i] = assist[sizeof(BytesType) - 1 - i];
		assist[sizeof(BytesType) - 1 - i] = c;
	}
	return *reinterpret_cast<BytesType*>(assist);
}

#if CS_IS_LITTLE_ENDIAN
inline uint64_t ntohll(uint64_t value)
{
   static union {
      uint64_t ull;
      uint8_t  c[8];
   } x;
   x.ull = value;

   int8_t c = 0;
   c = x.c[0]; x.c[0] = x.c[7]; x.c[7] = c;
   c = x.c[1]; x.c[1] = x.c[6]; x.c[6] = c;
   c = x.c[2]; x.c[2] = x.c[5]; x.c[5] = c;
   c = x.c[3]; x.c[3] = x.c[4]; x.c[4] = c;

   return x.ull;
}
#else
inline uint64_t ntohll(uint64_t value)
{
	return value;
}
#endif

inline uint64_t htonll(const uint64_t value)
{
   return ntohll(value);
}

}
