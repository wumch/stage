
#pragma once

#include "meta.hpp"

namespace staging {

template<std::size_t size>
class Hflp
{
public:
	std::string::size_type operator()(const std::string& data) const
	{
		unsigned int mask = 0;
		char* res = reinterpret_cast<char*>(mask);
		for (int i = 0; i < data.size(); ++i)
		{
			res[i & 3] ^= data[i];
		}
		return mask % data.size();
	}
};

}
