
#pragma once

#include "meta.hpp"
#include <string>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#ifdef __linux
#	include <sys/resource.h>
#	include <unistd.h>
#	include <sys/mman.h>
#else
#	error "system other than linux is currently not supported."
#endif

namespace staging {

inline std::string getNatIP()
{
	int fd, intrface;
	struct ifreq buf[16];
	struct ifconf ifc;
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (caddr_t) buf;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
	{
		if (!ioctl(fd, SIOCGIFCONF, reinterpret_cast<char *>(&ifc)))
		{
			intrface = ifc.ifc_len / sizeof(struct ifreq);
			while (intrface-- > 0)
			{
				if (!(ioctl(fd, SIOCGIFADDR, reinterpret_cast<char *>(&buf[intrface]))))
				{
					return std::string(inet_ntoa((reinterpret_cast<struct sockaddr_in*>(&buf[intrface].ifr_addr))->sin_addr));
				}
			}
		}
	}
	return std::string("");
}

}
