
#pragma once

#include "meta.hpp"

#ifndef CS_SINGLETON_METHOD_NAME
#   define CS_SINGLETON_METHOD_NAME instance
#endif

#ifndef CS_SINGLETON_FUNC_NAME
#   define CS_SINGLETON_FUNC_NAME getInstance
#endif

namespace stage {

#define mksingleton(T, ...)									\
friend T* stage::CS_SINGLETON_FUNC_NAME<T>();			\
public:														\
    static inline									\
	__VA_ARGS__ T* CS_SINGLETON_METHOD_NAME()				\
    {														\
        return stage::CS_SINGLETON_FUNC_NAME<T>();		\
    }

template<typename T> inline
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
