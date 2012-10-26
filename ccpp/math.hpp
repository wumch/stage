
#pragma once

#include "meta.hpp"

namespace staging {

template<typename ValType>
static CS_FORCE_INLINE ValType square(const ValType val)
{
	return val * val;
}

template<typename ResType, typename IterType>
static CS_FORCE_INLINE ResType sum(const IterType& begin, const IterType& end)
{
	ResType res = 0;
	for (IterType it = begin; it != end; ++it)
	{
		res += it->second;
	}
	return res;
}

}
