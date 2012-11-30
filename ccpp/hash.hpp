
#pragma once

#include "meta.hpp"
#ifdef __linux
#	include <limits.h>
#endif
#include <string>
#include <boost/static_assert.hpp>

namespace staging {

template<int int_bits> class IntTrait;
template<> class IntTrait<32> { public: typedef int32_t type; };
template<> class IntTrait<64> { public: typedef int64_t type; };

template<int int_bits> class UIntTrait;
template<> class UIntTrait<32> { public: typedef uint32_t type; };
template<> class UIntTrait<64> { public: typedef uint64_t type; };

template<int input_bits, typename output_bits = uint32_t> class HashProxyUInt;

// receive a 64bit int, output an 32bit int.
class Hash64to32
{
public:
	CS_FORCE_INLINE uint32_t operator()(uint64_t val) const
	{
		val = (~val) + (val << 18); // val = (val << 18) - val - 1;
		val ^= (val >> 31);
		val = (val + (val << 2)) + (val << 4);	// val = val * 21;
		val ^= (val >> 11);
		val += (val << 6);
		val ^= (val >> 22);
		return static_cast<uint32_t>(val);
	}

	template<typename T>
	CS_FORCE_INLINE uint32_t operator()(const T* ptr) const
	{
		return (*this)((uint64_t)ptr);
	}
};

// better for 32bit int.
template<typename IntType = uint32_t>
class HashTomasWang32
{
public:
	CS_FORCE_INLINE IntType operator()(IntType val) const
	{
		val = (~val) + (val << 15); // val = (val << 15) - val - 1;
		val ^= (val >> 12);
		val += (val << 2);
		val ^= (val >> 4);
		val = (val + (val << 3)) + (val << 11);
		val ^= (val >> 16);
		return val;
	}
};

// better for 64bit int.
template<typename IntType = uint64_t>
class HashTomasWang64
{
public:
	CS_FORCE_INLINE IntType operator()(IntType val) const
	{
		val = (~val) + (val << 21); // val = (val << 21) - val - 1;
		val ^= (val >> 24);
		val = (val + (val << 3)) + (val << 8); // val * 265
		val ^= (val >> 14);
		val = (val + (val << 2)) + (val << 4); // val * 21
		val ^= (val >> 28);
		val += (val << 31);
		return val;
	}
};

// hash(uint32, uint32, uint32) => uint32.
class HashMix96
{
public:
	CS_FORCE_INLINE uint32_t operator()(uint32_t a, uint32_t b, uint32_t c) const
	{
		a=a-b;  a=a-c;  a=a^(c >> 13);
		b=b-c;  b=b-a;  b=b^(a << 8);
		c=c-a;  c=c-b;  c=c^(b >> 13);
		a=a-b;  a=a-c;  a=a^(c >> 12);
		b=b-c;  b=b-a;  b=b^(a << 16);
		c=c-a;  c=c-b;  c=c^(b >> 5);
		a=a-b;  a=a-c;  a=a^(c >> 3);
		b=b-c;  b=b-a;  b=b^(a << 10);
		c=c-a;  c=c-b;  c=c^(b >> 15);
		return c;
	}
};

// better for 32bit int.
template<typename IntType = uint32_t>
class HashBobJenkins
{
public:
	CS_FORCE_INLINE IntType operator()(IntType val) const
	{
	   val = (val + 0x7ed55d16) + (val << 12);
	   val = (val ^ 0xc761c23c) ^ (val >> 19);
	   val = (val + 0x165667b1) + (val << 5);
	   val = (val + 0xd3a2646c) ^ (val << 9);
	   val = (val + 0xfd7046c5) + (val << 3); // <<和 +的组合是可逆的
	   val = (val ^ 0xb55a4f09) ^ (val >> 16);
	   return val;
	}
};

template<uint32_t mod, typename IntType = uint64_t>
class HashMod
{
public:
	CS_FORCE_INLINE IntType operator()(IntType val) const
	{
		return val % mod;
	}
};

template<uint8_t bits, typename StringType = std::string>
class HashHflp
{
	BOOST_STATIC_ASSERT(bits <= (sizeof(int) * CHAR_BIT));
public:
	CS_FORCE_INLINE uint32_t operator()(const StringType& data) const
	{
		unsigned int mask = 0;
		char* res = reinterpret_cast<char*>(&mask);
		for (uint i = 0; i < data.size(); ++i)
		{
			res[i & 3] ^= data[i];
		}
		return mask & ((1 << bits) - 1);
	}
};

template<uint8_t bits>
class HashBits
{
	BOOST_STATIC_ASSERT(bits < 64);
public:
	template<typename ValType> CS_FORCE_INLINE
	CS_FORCE_INLINE ValType operator()(ValType val) const
	{
		return val & ((1 << bits) - 1);
	}
};

// SDBM
class HashSDBM
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
class HashJS
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

///////////////////// compitable with old.

template<uint8_t bits, typename StringType = std::string>
class Hflp
{
	BOOST_STATIC_ASSERT(bits <= (sizeof(int) * CHAR_BIT));
public:
	CS_FORCE_INLINE uint32_t operator()(const StringType& data) const
	{
		unsigned int mask = 0;
		char* res = reinterpret_cast<char*>(&mask);
		for (uint i = 0; i < data.size(); ++i)
		{
			res[i & 3] ^= data[i];
		}
		return mask & ((1 << bits) - 1);
	}
};

template<uint8_t bits>
class BitsHash
{
	BOOST_STATIC_ASSERT(bits < 64);
public:
	template<typename ValType> CS_FORCE_INLINE
	CS_FORCE_INLINE ValType operator()(ValType val) const
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
