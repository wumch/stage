
#pragma once

#include "meta.hpp"
#include <boost/static_assert.hpp>
#include <string>

namespace staging {

template<std::size_t size>
class Hflp
{
public:
	std::string::size_type operator()(const std::string& data) const
	{
		unsigned int mask = 0;
		char* res = reinterpret_cast<char*>(&mask);
		for (int i = 0; i < data.size(); ++i)
		{
			res[i & 3] ^= data[i];
		}
		return mask % data.size();
	}
};

template<uint8_t bits>
class BitsHash
{
	BOOST_STATIC_ASSERT(bits < 64);
public:
	template<typename ValType> CS_FORCE_INLINE
	ValType operator()(ValType val) const
	{
		return val & ((1 << bits) - 1);
	}
};

// SDBM
class SDBMHash
{
public:
	CS_FORCE_INLINE uint32_t operator()(const char* const str, size_t size) const
	{
		unsigned int hash = 0;
		for (uint i = 0; i < size; ++i)
		{
			// equivalent to: hash = 65599*hash + (*str++);
			hash = (str[i]) + (hash << 6) + (hash << 16) - hash;
		}
		return (hash & 0x7FFFFFFF);
	}
};

// JS
class JSHash
{
public:
	CS_FORCE_INLINE uint32_t operator()(const std::string& str) const
	{
	    unsigned int hash = 1315423911;
	    for (uint i = 0; i < str.size(); ++i)
	    {
	        hash ^= ((hash << 5) + (str[i]) + (hash >> 2));
	    }
	    return (hash & 0x7FFFFFFF);
	}
};

}
