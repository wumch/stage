
/**
 * 所有 #include <外部文件> 的头文件 都必须保证 本文件被首先#include
 * 以下项目可以由本文件保证包含：
 * 	<cstdint>
 * 	<cstddef>
 * 	并保证 全局名称空间包含所有stdint。
 * 	CS_DEBUG = 1 时保证 #include <iostream>
 */

#pragma once

/**
 * 平台兼容原则：以不大的代价换取一定的兼容性；以首要目标平台0-overhead为前提。
 */

#ifndef CS_USE_WCS
#	define CS_USE_WCS	0
#endif

#ifndef NDEBUG
#	ifndef CS_DEBUG
#   	define CS_DEBUG		2
#	endif
#	ifndef CS_LOG_ON
#		define CS_LOG_ON	1
#	endif
#else
#	undef CS_DEBUG
#	define CS_DEBUG			0
#	undef CS_LOG_ON
#	define CS_LOG_ON		0
#endif

#include <boost/cstdint.hpp>
#include <cstddef>

#define CS_EXIT_STATUS_FAILED 1

#ifdef __cplusplus
#	define CS_PREF_STD(symbol)		std::symbol
#else
#	define CS_PREF_STD(symbol)		symbol
#endif

#if CS_USE_WCS
#	define CS_STDIN		::std::wcin
#	define CS_STDOUT	::std::wcout
#	define CS_STDERR	::std::wcerr
#else
#	define CS_STDIN		::std::cin
#	define CS_STDOUT	::std::cout
#	define CS_STDERR	::std::cerr
#endif

#define CS_OC_BLACK(...)	"\033[32;30;5m" << __VA_ARGS__ << "\033[0m"
#define CS_OC_BLUE(...)		"\033[32;34;5m" << __VA_ARGS__ << "\033[0m"
#define CS_OC_RED(...)		"\033[32;31;5m" << __VA_ARGS__ << "\033[0m"
#define CS_OC_GREEN(...)	"\033[32;49;5m" << __VA_ARGS__ << "\033[0m"


// line-seperator, should only use for IO..
#ifdef __linux
#	define CS_LINESEP	'\n'
#else
#	ifdef(__windows)
#		define CS_LINESEP	"\r\n"
#	else
#		define CS_LINESEP	::std::endl
#	endif
#endif

#if CS_DEBUG
#   include <iostream>
#   if CS_DEBUG > 1
#		if CS_USE_WCS
#			define CS_OUT(ostream, ...)													\
				ostream << __FILE__ << ":" << __LINE__					\
					<< ":" << __FUNCTION__ << "()" << ":\t"							\
					<< __VA_ARGS__														\
					<< ::std::endl;
#		else
#       	define CS_OUT(ostream, ...)													\
				ostream << CS_OC_BLACK(__FILE__ << ":" << __LINE__)		\
					<< ":" << CS_OC_BLUE(__FUNCTION__ << "()") << ":\t"				\
					<< CS_OC_GREEN(__VA_ARGS__)											\
					<< ::std::endl;
#		endif
#   else
#		define CS_OUT(ostream, ...)	ostream << __VA_ARGS__ << ::std::endl;
#   endif
#else
#	define CS_OUT(ostream, ...)
#endif

#define CS_SAY(...)		CS_OUT(CS_STDOUT, __VA_ARGS__)

// NOTE: 要自行 #include <iostream>
#define CS_ECHO(...)	CS_STDOUT << __VA_ARGS__ << ::std::endl;
#define CS_ERR(...)		CS_STDERR << __VA_ARGS__ << ::std::endl;

// #define CS_DUMP(...)	CS_OUT(CS_STDOUT, GOL_OC_BLUE(#__VA_ARGS__) << ": " << GOL_OC_GREEN(__VA_ARGS__))

#define CS_DUMP(...)																	\
	CS_OUT(CS_STDOUT,  CS_OC_BLACK(__FILE__ ":" << __LINE__)									\
		<< "\t" << CS_OC_BLUE(__FUNCTION__) << CS_OC_BLUE("(...)") << ":\t"				\
		<< CS_OC_RED(#__VA_ARGS__) << ":[" << CS_OC_GREEN(__VA_ARGS__) << "]")
#define CS_DUMP_N(...) 																	\
	CS_OUT(CS_STDOUT, CS_OC_BLACK(__FILE__ ":") << CS_OC_BLACK(__LINE__) 					\
		<< "\t" << CS_OC_BLUE(__FUNCTION__) << CS_OC_BLUE("(...)") << ":\t" 			\
		<< CS_OC_RED(#__VA_ARGS__) << ":" << ::std::endl << CS_OC_GREEN(__VA_ARGS__))


// #define CS_ERR(msg)
//	CS_STDERR << msg << ::std::endl
// --- 要自行 #include <cstdlib>
#define CS_DIE(msg)																		\
	CS_ERR(msg); 																		\
    ::std::exit(CS_EXIT_STATUS_FAILED)

#ifndef CS_USED
#	ifdef __GNUC__
#		define CS_USED __attribute__((__used__))
#	else
#		define CS_USED
#	endif
#endif

#ifndef CS_ATTR_CONST
#	ifdef __GNUC__
#		define CS_ATTR_CONST __attribute__((__const__))
#	else
#		define CS_ATTR_CONST
#	endif
#endif

#ifndef CS_MUST_CHECK
#	ifdef __GNUC__
#		define CS_MUST_CHECK __attribute__((warn_unused_result))
#	else
#		define CS_MUST_CHECK
#	endif
#endif

#ifndef CS_DEPRECATED
#	ifdef __GNUC__
#		define CS_DEPRECATED __attribute__((deprecated))
#	else
#		define CS_DEPRECATED
#	endif
#endif

#ifndef CS_FORCE_INLINE
#   ifdef __GNUC__
#       define CS_FORCE_INLINE __attribute__((always_inline))
#   else
#       define CS_FORCE_INLINE inline
#   endif
#endif

#if !defined(CS_LIKELY) && !defined(CS_UNLIKELY)
#	ifdef __GNUC__
#		define CS_LIKELY(...)		__builtin_expect(!!(__VA_ARGS__), 1)
#		define CS_UNLIKELY(...)		__builtin_expect(!!(__VA_ARGS__), 0)
#		define CS_BLIKELY(...)		__builtin_expect((__VA_ARGS__), true)
#		define CS_BUNLIKELY(...)	__builtin_expect((__VA_ARGS__), false)
#	else
#		define CS_LIKELY(expr)		(expr)
#		define CS_UNLIKELY(expr)	(expr)
#		define CS_BLIKELY(expr)		(expr)
#		define CS_BUNLIKELY(expr)	(expr)
#	endif
#endif

#ifndef CS_PREFETCH
#	ifdef __GNUC__
#		define CS_PREFETCH(addr,rw,clean) __builtin_prefetch(addr,rw,clean)
#	else
#		define CS_PREFETCH(addr,rw,clean)
#	endif
#endif

#define CS_RETURN_IF(cond, ...)		\
if (cond)							\
{									\
	return __VA_ARGS__;				\
}
