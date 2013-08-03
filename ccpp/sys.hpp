
#pragma once

#include "meta.hpp"
#ifdef __linux
#	include <sys/resource.h>
#	include <unistd.h>
#	include <sys/mman.h>
#else
typedef int64_t rlim_t;
#	error "system other than linux is currently not supported."
#endif

namespace staging {

static CS_FORCE_INLINE std::size_t getCpuNum()
{
#ifdef __linux
	return sysconf(_SC_NPROCESSORS_CONF);
#else
	return 1;
#endif
}

static CS_FORCE_INLINE rlim_t getRlimitCur(int resource)
{
#ifdef __linux
	struct rlimit limit;
	getrlimit(resource, &limit);
	return limit.rlim_cur;
#else
	return 0;
#endif
}

static CS_FORCE_INLINE rlim_t getRlimitMax(int resource)
{
#ifdef __linux
	struct rlimit limit;
	getrlimit(resource, &limit);
	return limit.rlim_max;
#else
	return 0;
#endif
}

static CS_FORCE_INLINE int setRlimit(int resource, rlim_t expect)
{
#ifdef __linux
	struct rlimit limit;
	getrlimit(resource, &limit);
	if (limit.rlim_cur != expect)
	{
		limit.rlim_cur = expect;
		return setrlimit(resource, &limit);
	}
	return 0;
#else
	return 0;
#endif
}

}
