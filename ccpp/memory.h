
#pragma once

#include <stdio.h>
#ifdef __cplusplus
#	include <boost/cstdint.hpp>
#else
#	include <stdint.h>
#endif
#include <string.h>
#include <limits.h>
#include <boost/preprocessor/seq/elem.hpp>

#ifdef __cplusplus
#	define CS_PREF_STD(symbol)		std::symbol
#else
#	define CS_PREF_STD(symbol)		symbol
#endif

inline int cs_memcmp(const void* s1, const void* s2, CS_PREF_STD(size_t) n)
{
	printf("%s called\n", __FUNCTION__);
	return CS_PREF_STD(memcmp)(s1, s2, n);
}

#define CS_CONST_STRLEN(s)									\
	(sizeof(s) / (CHAR_BIT >> 3) - 1)

#define CS_IS_BUILTIN_TYPE(size)							\
	(__builtin_constant_p(size) && (size == 1 || size == 2 || size == 4 || size == 8 || size == 16))

#define CS_ITYPES											\
	(0)(CS_PREF_STD(int8_t))(CS_PREF_STD(int16_t))(0)(CS_PREF_STD(int32_t))(0)(0)(0)(CS_PREF_STD(int64_t))(0)(0)(0)(0)(0)(0)(0)(long double)

#define CS_GET_ITYPE_BY_SIZE(size)							\
	BOOST_PP_SEQ_ELEM(size, CS_ITYPES)

#define CS_MEMCMP(s1, s2, n)								\
	(CS_IS_BUILTIN_TYPE(n) ? (*(CS_GET_ITYPE_BY_SIZE(n)*)(s1) > *(CS_GET_ITYPE_BY_SIZE(n)*)(s2) ? 1 : (*(CS_GET_ITYPE_BY_SIZE(n)*)(s1) == *(CS_GET_ITYPE_BY_SIZE(n)*)(s2) ? 0 : -1)) : cs_memcmp(s1, s2, n))



// for c only
#define _CS_MEMCMP(s1, s2, n)																											\
	(__builtin_constant_p(n) ?																											\
		((n == 1) ? (*(int8_t*)(s1) > *(int8_t*)(s2) ? 1 : (*(int8_t*)(s1) == *(int8_t*)(s2) ? 0 : -1))									\
				: ((n == 2) ? (*(int16_t*)(s1) > *(int16_t*)(s2) ? 1 : (*(int16_t*)(s1) == *(int16_t*)(s2) ? 0 : -1))					\
						: ((n == 4) ? (*(int32_t*)(s1) > *(int32_t*)(s2) ? 1 : (*(int32_t*)(s1) == *(int32_t*)(s2) ? 0 : -1))			\
								: ((n == 8) ? (*(int64_t*)(s1) > *(int64_t*)(s2) ? 1 : (*(int64_t*)(s1) == *(int64_t*)(s2) ? 0 : -1))	\
										: cs_memcmp(s1, s2, n))))) : cs_memcmp(s1, s2, n))
										  
