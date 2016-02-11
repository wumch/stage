
#pragma once

#include "meta.hpp"
#include <cstdarg>
#include <boost/operators.hpp>
#include "misc.hpp"

namespace stage {

template<typename ValueType>
static inline bool between(ValueType val, ValueType min, ValueType max)
{
	return min <= val && val <= max;
}

template<typename ValueType>
static inline bool between_open(ValueType val, ValueType min, ValueType max)
{
	return min < val && val < max;
}

template<typename ValType>
static inline ValType square(ValType val)
{
	return val * val;
}

template<typename ResType, typename IterType>
static inline ResType sum(const IterType& begin, const IterType& end)
{
	ResType res = 0;
	for (IterType it = begin; it != end; ++it)
	{
		res += it->second;
	}
	return res;
}

/*
 * CS_IN(needle, elem_1, elem_2, ...)
 * NOTE: designed for only numeric-types.
 */
template<typename ValueType>
static inline bool in(ValueType needle)
{
   return false;
}

template<typename ValueType>
static inline bool in(ValueType needle,
    ValueType elem_1)
{
    return needle == elem_1;
}

template<typename ValueType>
static inline bool in(ValueType needle,
    ValueType elem_1,
    ValueType elem_2)
{
    return needle == elem_1 || needle == elem_2;
}

template<typename ValueType>
static inline bool in(ValueType needle,
    ValueType elem_1,
    ValueType elem_2,
    ValueType elem_3)
{
    return needle == elem_1 || needle == elem_2 || needle == elem_3;
}

template<typename ValueType>
static inline bool in(ValueType needle,
    ValueType elem_1,
    ValueType elem_2,
    ValueType elem_3,
    ValueType elem_4)
{
    return needle == elem_1 || needle == elem_2 || needle == elem_3
        || needle == elem_4;
}

template<typename ValueType>
static inline bool in(ValueType needle,
    ValueType elem_1,
    ValueType elem_2,
    ValueType elem_3,
    ValueType elem_4,
    ValueType elem_5)
{
    return needle == elem_1 || needle == elem_2 || needle == elem_3
        || needle == elem_4 || needle == elem_5;
}

template<typename ValueType>
static inline bool in(ValueType needle,
    ValueType elem_1,
    ValueType elem_2,
    ValueType elem_3,
    ValueType elem_4,
    ValueType elem_5,
    ValueType elem_6)
{
    return needle == elem_1 || needle == elem_2 || needle == elem_3
        || needle == elem_4 || needle == elem_5 || needle == elem_6;
}

template<typename ValueType>
static inline bool in(const ValueType needle, int argc, ...)
{
    std::va_list argp;
    va_start(argp, argc);
    for (int i = 0; i < argc; ++i)
    {
        if (va_arg(argp, ValueType) == needle)
        {
            return true;
        }
    }
    va_end(argp);
    return false;
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

#define CS_IN(needle, ...)                                                      \
    (CS_PP_NARG(__VA_ARGS__) < 7 ?                                              \
    stage::in<int64_t>(needle, __VA_ARGS__) :                                          \
    stage::in<int64_t>(needle, CS_PP_NARG(__VA_ARGS__), __VA_ARGS__))
