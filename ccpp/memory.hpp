
#pragma once

#include "meta.hpp"
#include <memory.h>

#ifdef __GNUC__

#include <boost/preprocessor/seq/elem.hpp>
#include <boost/static_assert.hpp>

#define _CS_IS_BUILTIN_TYPE(size)												\
	(__builtin_constant_p(size) && (size == 1 || size == 2 || size == 4 || size == 8 || size == 16))

#define _CS_ITYPES																\
	(0)(int8_t)(int16_t)(0)(int32_t)(0)(0)(0)(int64_t)(0)(0)(0)(0)(0)(0)(0)(long double)

#define CS_GET_ITYPE_BY_SIZE(size)												\
	BOOST_PP_SEQ_ELEM(size, _CS_ITYPES)

#define CS_MEMCMP(s1, s2, n)													\
	(_CS_IS_BUILTIN_TYPE(n) ? (*(CS_GET_ITYPE_BY_SIZE(n)*)(s1) > *(CS_GET_ITYPE_BY_SIZE(n)*)(s2) ? 1 : (*(CS_GET_ITYPE_BY_SIZE(n)*)(s1) == *(CS_GET_ITYPE_BY_SIZE(n)*)(s2) ? 0 : -1)) : cs_memcmp(s1, s2, n))

#define CS_MEMCMP(s1, s2, n)													\
	(_CS_IS_BUILTIN_TYPE(n) ? (*(CS_GET_ITYPE_BY_SIZE(n)*)(s1) > *(CS_GET_ITYPE_BY_SIZE(n)*)(s2) ? 1 : (*(CS_GET_ITYPE_BY_SIZE(n)*)(s1) == *(CS_GET_ITYPE_BY_SIZE(n)*)(s2) ? 0 : -1)) : cs_memcmp(s1, s2, n))

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

namespace staging {

template<size_t dwords> CS_FORCE_INLINE
int memcmp4(const int32_t* const s1, const int32_t* const s2)
{
	return ::memcmp(s1, s2, dwords * 4);
}
template<> CS_FORCE_INLINE
int memcmp4<1>(const int32_t* const s1, const int32_t* const s2)
{
	return *reinterpret_cast<const int32_t*>(s1) == *reinterpret_cast<const int32_t*>(s2);
}

template<> CS_FORCE_INLINE
int memcmp4<2>(const int32_t* const s1, const int32_t* const s2)
{
	return *reinterpret_cast<const int64_t*>(s1) == *reinterpret_cast<const int64_t*>(s2);
}

template<> CS_FORCE_INLINE
int memcmp4<3>(const int32_t* const s1, const int32_t* const s2)
{
	return *reinterpret_cast<const int64_t*>(s1) == *reinterpret_cast<const int64_t*>(s2)
			&& s1[2] == s2[2];
}

template<> CS_FORCE_INLINE
int memcmp4<4>(const int32_t* const s1, const int32_t* const s2)
{
	return *reinterpret_cast<const int64_t*>(s1) == *reinterpret_cast<const int64_t*>(s2)
			&& reinterpret_cast<const int64_t*>(s1)[1] == reinterpret_cast<const int64_t*>(s2)[1];
}

template<> CS_FORCE_INLINE
int memcmp4<5>(const int32_t* const s1, const int32_t* const s2)
{
	return *reinterpret_cast<const int64_t*>(s1) == *reinterpret_cast<const int64_t*>(s2)
			&& reinterpret_cast<const int64_t*>(s1)[1] == reinterpret_cast<const int64_t*>(s2)[1]
		   && s1[4] == s2[4];
}

template<> CS_FORCE_INLINE
int memcmp4<6>(const int32_t* const s1, const int32_t* const s2)
{
	return *reinterpret_cast<const int64_t*>(s1) == *reinterpret_cast<const int64_t*>(s2)
			&& reinterpret_cast<const int64_t*>(s1)[1] == reinterpret_cast<const int64_t*>(s2)[1]
			&& reinterpret_cast<const int64_t*>(s1)[2] == reinterpret_cast<const int64_t*>(s2)[2];
}

template<> CS_FORCE_INLINE
int memcmp4<7>(const int32_t* const s1, const int32_t* const s2)
{
	return *reinterpret_cast<const int64_t*>(s1) == *reinterpret_cast<const int64_t*>(s2)
			&& reinterpret_cast<const int64_t*>(s1)[1] == reinterpret_cast<const int64_t*>(s2)[1]
			&& reinterpret_cast<const int64_t*>(s1)[2] == reinterpret_cast<const int64_t*>(s2)[2]
		   && s1[6] == s2[6];
}

// ------ memcpy4 ------
template<size_t dwords> CS_FORCE_INLINE
void memcpy4(void* const s1, const void* const s2)
{
	::memcpy(s1, s2, dwords);
}

template<> CS_FORCE_INLINE
void memcpy4<4>(void* const s1, const void* const s2)
{
	*reinterpret_cast<int32_t*>(s1) = *reinterpret_cast<const int32_t*>(s2);
}

template<> CS_FORCE_INLINE
void memcpy4<8>(void* const s1, const void* const s2)
{
	*reinterpret_cast<int64_t*>(s1) = *reinterpret_cast<const int64_t*>(s2);
}

template<> CS_FORCE_INLINE
void memcpy4<12>(void* const s1, const void* const s2)
{
	*reinterpret_cast<int64_t*>(s1) = *reinterpret_cast<const int64_t*>(s2);
	reinterpret_cast<int32_t*>(s1)[2] = reinterpret_cast<const int32_t*>(s2)[2];
}

template<> CS_FORCE_INLINE
void memcpy4<16>(void* const s1, const void* const s2)
{
	*reinterpret_cast<int64_t*>(s1) = *reinterpret_cast<const int64_t*>(s2);
	reinterpret_cast<int64_t*>(s1)[1] = reinterpret_cast<const int64_t*>(s2)[1];
}

template<> CS_FORCE_INLINE
void memcpy4<20>(void* const s1, const void* const s2)
{
	*reinterpret_cast<int64_t*>(s1) = *reinterpret_cast<const int64_t*>(s2);
	reinterpret_cast<int64_t*>(s1)[1] = reinterpret_cast<const int64_t*>(s2)[1];
	reinterpret_cast<int32_t*>(s1)[4] = reinterpret_cast<const int32_t*>(s2)[4];
}

template<> CS_FORCE_INLINE
void memcpy4<24>(void* const s1, const void* const s2)
{
	*reinterpret_cast<int64_t*>(s1) = *reinterpret_cast<const int64_t*>(s2);
	reinterpret_cast<int64_t*>(s1)[1] = reinterpret_cast<const int64_t*>(s2)[1];
	reinterpret_cast<int64_t*>(s1)[2] = reinterpret_cast<const int64_t*>(s2)[2];
}

template<> CS_FORCE_INLINE
void memcpy4<28>(void* const s1, const void* const s2)
{
	*reinterpret_cast<int64_t*>(s1) = *reinterpret_cast<const int64_t*>(s2);
	reinterpret_cast<int64_t*>(s1)[1] = reinterpret_cast<const int64_t*>(s2)[1];
	reinterpret_cast<int64_t*>(s1)[2] = reinterpret_cast<const int64_t*>(s2)[2];
	reinterpret_cast<int32_t*>(s1)[6] = reinterpret_cast<const int32_t*>(s2)[6];
}

template<> CS_FORCE_INLINE
void memcpy4<32>(void* const s1, const void* const s2)
{
	*reinterpret_cast<int64_t*>(s1) = *reinterpret_cast<const int64_t*>(s2);
	reinterpret_cast<int64_t*>(s1)[1] = reinterpret_cast<const int64_t*>(s2)[1];
	reinterpret_cast<int64_t*>(s1)[2] = reinterpret_cast<const int64_t*>(s2)[2];
	reinterpret_cast<int64_t*>(s1)[3] = reinterpret_cast<const int64_t*>(s2)[3];
}

}
