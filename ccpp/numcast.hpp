
#pragma once

#include "meta.hpp"
#include <string.h>

namespace staging {

class NumCast
{
public:
	template<typename char_t>
	static size_t strtoul(const char_t* const str, size_t bits)
	{
		size_t res = 0;
		for (size_t i = 0; i < bits; ++i)
		{
			if (CS_BLIKELY('0' <= str[i] && str[i] <= '9'))
			{
				res *= 10;
				res += (str[i] - 48);
			}
			else
			{
				return res;
			}
		}
		return res;
	}

	template<typename char_t>
	static size_t strtoul(const char_t* const str)
	{
		size_t res = 0;
		for (size_t i = 0; i < 30; ++i)
		{
			if (CS_BLIKELY('0' <= str[i] && str[i] <= '9'))
			{
				res *= 10;
				res += (str[i] - '0');
			}
			else
			{
				return res;
			}
		}
		return res;
	}


	template<typename char_t>
	static size_t strtoul_comma(const char_t* const str, size_t* const consumed_bytes = NULL)
	{
		size_t res = 0, i = 0;
		for ( ; i < 30; ++i)
		{
			if (CS_BLIKELY('0' <= str[i] && str[i] <= '9'))
			{
				res *= 10;
				res += (str[i] - '0');
			}
			else if (CS_BUNLIKELY(str[i] != ','))
			{
				if (consumed_bytes != NULL)
				{
					*consumed_bytes = i;
				}
				return res;
			}
		}
		if (consumed_bytes != NULL)
		{
			*consumed_bytes = i;
		}
		return res;
	}


	template<typename char_t>
	static size_t ultostr(size_t n, char_t* const str)
	{
		char_t s[32];
		size_t len = sizeof(s) / sizeof(char_t), i = len - 1;
		s[i] = (n % 10) + '0';
		n /= 10;
		while (n > 0)
		{
			s[--i] = (n % 10) + '0';
			n /= 10;
		}
		memcpy(str, s + i, len - i);
		return len - i;
	}
};

}
