
#pragma once

#include "meta.hpp"
#include <boost/operators.hpp>

namespace staging {

template<typename ValueType>
static CS_FORCE_INLINE bool between(ValueType val, ValueType min, ValueType max)
{
	return min <= val && val <= max;
}

template<typename ValueType>
static CS_FORCE_INLINE bool between_open(ValueType val, ValueType min, ValueType max)
{
	return min < val && val < max;
}

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

// infinity
template<typename T = double>
class Infinity
	: public ::boost::less_than_comparable2<T, Infinity<T> >
	  , public ::boost::equality_comparable2<T, Infinity<T> >
{
public:
	typedef T value_type;
	typedef value_type param_type;
};

template<typename T>
static inline bool operator<(typename Infinity<T>::param_type val, const Infinity<T>& inf)
{
	return true;
}

template<typename T>
static inline bool operator>(typename Infinity<T>::param_type val, const Infinity<T>& inf)
{
	return false;
}

template<typename T>
static inline bool operator==(typename Infinity<T>::param_type val, const Infinity<T>& inf)
{
	return false;
}

template<typename T>
static inline bool operator==(const Infinity<T>& val, const Infinity<T>& inf)
{
	return true;
}

// infinitesimal
template<typename T = double>
class Infinitesimal
	: public ::boost::less_than_comparable2<T, Infinitesimal<T> >
	  , public ::boost::equality_comparable2<T, Infinitesimal<T> >
{
public:
	typedef T value_type;
	typedef value_type param_type;
};

template<typename T>
static inline bool operator<(typename Infinitesimal<T>::param_type val, const Infinitesimal<T>& inf)
{
	return false;
}

template<typename T>
static inline bool operator>(typename Infinitesimal<T>::param_type val, const Infinitesimal<T>& inf)
{
	return true;
}

template<typename T>
static inline bool operator==(typename Infinitesimal<T>::param_type val, const Infinitesimal<T>& inf)
{
	return false;
}

template<typename T>
static inline bool operator==(const Infinitesimal<T>& val, const Infinitesimal<T>& inf)
{
	return true;
}

}
