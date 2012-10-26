
#pragma once

#include "meta.hpp"

namespace staging {

template<int tagid_> class AutoIncrTag {};

class AutoIncrDefaultTrait
{
public:
	typedef uint32_t inttype;
	static const inttype begin = 0;
};

template<typename TagType_,
	typename IntType_ = AutoIncrDefaultTrait::inttype,
	IntType_ begin_ = AutoIncrDefaultTrait::begin
>
class AutoIncr
{
public:
	typedef IntType_ inttype;
	typedef TagType_ TagType;

public:
	static AutoIncr* instance()
	{
		static AutoIncr* instance_ = new AutoIncr;
		return instance_;
	}

	inttype gen()
	{
		return ++cur;
	}

	inttype last()
	{
		return cur;
	}

private:
	inttype cur;

	AutoIncr()
		: cur(begin_ - 1)
	{

	}
};

}
