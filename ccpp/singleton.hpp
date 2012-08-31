
#pragma once

#include "meta.hpp"

namespace staging {

#define mksingleton(T, ...)									\
friend T* staging::getInstance<T>();						\
public:														\
    static CS_FORCE_INLINE									\
	__VA_ARGS__ T* getInstance()							\
    {														\
        return staging::getInstance<T>();					\
    }

template<typename T> CS_FORCE_INLINE
T* getInstance()
{
    static T instance;
    return &instance;
};

template<typename T>
class Singleton
{
public:
    static T* getInstance()
    {
        static T instance;
        return &instance;
    }
};

template<typename T>
class StaticSingleton
{
private:
	static T instance;
public:
	static T* getInstance()
	{
		return &instance;
	}
};

}
