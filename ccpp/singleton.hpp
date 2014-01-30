
#pragma once

#include "meta.hpp"

#ifndef CS_SINGLETON_METHOD_NAME
#   define CS_SINGLETON_METHOD_NAME instance
#endif

#ifndef CS_SINGLETON_FUNC_NAME
#   define CS_SINGLETON_FUNC_NAME getInstance
#endif

namespace staging {

#define mksingleton(T, ...)									\
friend T* staging::CS_SINGLETON_FUNC_NAME<T>();			\
public:														\
    static CS_FORCE_INLINE									\
	__VA_ARGS__ T* CS_SINGLETON_METHOD_NAME()				\
    {														\
        return staging::CS_SINGLETON_FUNC_NAME<T>();		\
    }

template<typename T> CS_FORCE_INLINE
T* CS_SINGLETON_FUNC_NAME()
{
    static T _instance;
    return &_instance;
};

template<typename T>
class Singleton
{
public:
    static T* CS_SINGLETON_METHOD_NAME()
    {
        static T _instance;
        return &_instance;
    }
};

template<typename T>
class StaticSingleton
{
private:
	static T _instance;
public:
	static T* CS_SINGLETON_METHOD_NAME()
	{
		return &_instance;
	}
};

}
