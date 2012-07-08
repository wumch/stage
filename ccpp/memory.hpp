
#pragma once

#include "meta.hpp"
#include <memory.h>

#ifdef __GNUC__

#include <limits.h>
#include <boost/preprocessor/seq/elem.hpp>

#define CS_CONST_STRLEN(s)			(sizeof(s) / (CHAR_BIT >> 3) - 1)

#define _CS_IS_BUILTIN_TYPE(size)												\
	(__builtin_constant_p(size) && (size == 1 || size == 2 || size == 4 || size == 8 || size == 16))

#define _CS_ITYPES																\
	(0)(CS_PREF_STD(int8_t))(CS_PREF_STD(int16_t))(0)(CS_PREF_STD(int32_t))(0)(0)(0)(CS_PREF_STD(int64_t))(0)(0)(0)(0)(0)(0)(0)(long double)

#define _CS_GET_ITYPE_BY_SIZE(size)												\
	BOOST_PP_SEQ_ELEM(size, _CS_ITYPES)

#define CS_MEMCMP(s1, s2, n)													\
	(_CS_IS_BUILTIN_TYPE(n) ? (*(_CS_GET_ITYPE_BY_SIZE(n)*)(s1) > *(_CS_GET_ITYPE_BY_SIZE(n)*)(s2) ? 1 : (*(_CS_GET_ITYPE_BY_SIZE(n)*)(s1) == *(_CS_GET_ITYPE_BY_SIZE(n)*)(s2) ? 0 : -1)) : cs_memcmp(s1, s2, n))

// for c only
#define _CS_MEMCMP(s1, s2, n)																											\
	(__builtin_constant_p(n) ?																											\
		((n == 1) ? (*(int8_t*)(s1) > *(int8_t*)(s2) ? 1 : (*(int8_t*)(s1) == *(int8_t*)(s2) ? 0 : -1))									\
				: ((n == 2) ? (*(int16_t*)(s1) > *(int16_t*)(s2) ? 1 : (*(int16_t*)(s1) == *(int16_t*)(s2) ? 0 : -1))					\
						: ((n == 4) ? (*(int32_t*)(s1) > *(int32_t*)(s2) ? 1 : (*(int32_t*)(s1) == *(int32_t*)(s2) ? 0 : -1))			\
								: ((n == 8) ? (*(int64_t*)(s1) > *(int64_t*)(s2) ? 1 : (*(int64_t*)(s1) == *(int64_t*)(s2) ? 0 : -1))	\
										: memcmp(s1, s2, n))))) : memcmp(s1, s2, n))
										  

#else		// ifdef __GNUC__

#define CS_MEMCMP(s1, s2, n)	memcmp(s1, s2, n)

#endif		// ifdef __GNUC__
