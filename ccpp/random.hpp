
#pragma once

#include "meta.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/noncopyable.hpp>

namespace staging {

template<typename EngineType_ = boost::mt19937, typename result_type = uint32_t>
class RandNumberGenerator
{
public:
	typedef EngineType_ EngineType;
	typedef result_type res_type;

protected:
	typedef boost::uniform_int<res_type> Uniformer;
	Uniformer uniformer;
	EngineType engine;

public:
    RandNumberGenerator(std::size_t seed, res_type min, res_type max)
    	: uniformer(min, max), engine(seed)
    {}

    inline res_type operator()()
    {
        return uniformer(engine);
    }

    inline res_type operator()(res_type min, res_type max)
    {
        return Uniformer(min, max)(engine);
    }

    inline EngineType& getEngine()
    {
        return engine;
    }
};

template<typename result_type = uint32_t>
class RandomAssister
{
public:
	typedef result_type res_type;

protected:
	typedef RandNumberGenerator<boost::mt19937, res_type> Generator;

	const res_type lower;	// 只能是1，无法定制
	const res_type upper;
	Generator gener;

	res_type tmp_random;	// 辅助参数

public:
    RandomAssister()
    	: lower(0), upper(0), gener(time(NULL), lower, upper)
    {}

    RandomAssister(uint32_t seed)
    	: lower(0), upper(0), gener(seed, lower, upper)
    {}

    RandomAssister(res_type max, uint32_t seed)
		: lower(0), upper(max), gener(seed, lower, upper)
	{
	}

    RandomAssister(res_type min, res_type max, uint32_t seed)
		: lower(min), upper(max), gener(seed, lower, upper)
	{
	}

    inline res_type rand()
    {
        return gener();
    }

    inline res_type randDirect(res_type min, res_type max)
    {
    	return gener(min, max);
    }

    // 随机返回 整数 in [min,max]
    inline res_type rand(res_type min, res_type max)
    {
    	return genRes(min, max, gener());
    }

    // 随机返回 整数 in [min,max]，以 luky/upper_bound 的几率返回max
    inline res_type rand(res_type min, res_type max, res_type lucky)
    {
    	tmp_random = gener();
    	if (CS_BUNLIKELY(tmp_random <= lucky))
    	{
    		return max;
    	}
    	else if (CS_BUNLIKELY(lucky < 0 && tmp_random <= -lucky))
    	{
    		return min;
    	}
    	else
    	{
    		return rand(min, max);
    	}
    }

    // 检测：以 1 / denominator 的概率返回 true
    inline bool detect(res_type denominator)
    {
    	return denominator == 1 ? true : detectDirect(denominator);
    }

    // 检测：以 numerator / denominator 的概率返回 true
    inline bool detect(res_type numerator, res_type denominator)
    {
    	return CS_BUNLIKELY(denominator <= numerator) ?
    		true : detectDirect(numerator, denominator);
    }

    // 直接检测
    inline bool detectDirect(res_type denominator)
    {
    	return gener() * denominator <= upper;
    }

    // 直接检测
    inline bool detectDirect(res_type numerator, res_type denominator)
    {
    	return gener() * denominator <= numerator * upper;
    }

protected:	// 内部函数
    inline res_type genRes(res_type min, res_type max, res_type rand_val)
    {
    	return min + static_cast<res_type>((max - min) * rand_val / upper);
    }

};

} /* namespace staging */
