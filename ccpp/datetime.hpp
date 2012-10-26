
#pragma once

#include "meta.hpp"
#include <string>
#include <ctime>

namespace staging {

static inline std::string datetime(time_t time = std::time(NULL))
{
	static const char* const format = "%Y-%m-%d %H:%M:%S";
	static const int max_chars = 20;
	static char buf[max_chars];

	std::tm* local_tm = std::localtime(&time);
	size_t writen = strftime(buf, max_chars, format, local_tm);

	return std::string(reinterpret_cast<const char*>(buf), writen);
}

}
