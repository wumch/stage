
#pragma once

#include "meta.hpp"

// 宏flag：IS_LITTLE_ENDIAN
#include <endian.h>
#if __BYTE_ORDER == __LITTLE_ENDIAN
#   define CS_IS_LITTLE_ENDIAN 1
#else
#	warning "working on big-endian machine."
#   define CS_IS_LITTLE_ENDIAN 0
#endif

// 工具宏：取宏参数个数(NOTE:1~63个)
#define CS_PP_NARG(...) 	_CS_PP_NARG(__VA_ARGS__, _CS_PP_RSEQ_N())
#define _CS_PP_NARG(...)    _CS_PP_ARG_N(__VA_ARGS__)
#define _CS_PP_ARG_N(																	\
     _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, 											\
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, 											\
    _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, 											\
    _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, 											\
    _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, 											\
    _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, 											\
    _61,_62,_63,  N, ...) N
#define _CS_PP_RSEQ_N()																	\
    63,62,61,60,                   														\
    59,58,57,56,55,54,53,52,51,50, 														\
    49,48,47,46,45,44,43,42,41,40, 														\
    39,38,37,36,35,34,33,32,31,30, 														\
    29,28,27,26,25,24,23,22,21,20, 														\
    19,18,17,16,15,14,13,12,11,10, 														\
     9, 8, 7, 6, 5, 4, 3, 2, 1, 0

// 工具宏：定义enum，同时建立等价 std::tr1::array (不自动包含)
#define ENUM_WITH_ARRAY(ENUM_NAME, ARRAY_NAME, ...) 														\
	enum ENUM_NAME {__VA_ARGS__}; 																			\
	const std::tr1::array<ENUM_NAME,CS_PP_NARG(__VA_ARGS__)> ARRAY_NAME = {{__VA_ARGS__}};
// --- 附加 std::tr1::array<>.size()
#define ENUM_ARRAY_TYPE_SIZE(ENUM_NAME, ARRAY_SIZE_NAME, ELEM_TYPE, ARRAY_TYPE_NAME, ARRAY_NAME, ...) 		\
	enum ENUM_NAME {__VA_ARGS__}; 																			\
	const std::size_t ARRAY_SIZE_NAME = CS_PP_NARG(__VA_ARGS__); 											\
	typedef std::tr1::array<ELEM_TYPE, ARRAY_SIZE_NAME> ARRAY_TYPE_NAME; 									\
	const ARRAY_TYPE_NAME ARRAY_NAME = {{__VA_ARGS__}};
