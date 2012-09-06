
#pragma once

#include <new>
#include <malloc.h>
#include <stddef.h>
#include <list>
#include <bits/functexcept.h>

#ifndef force_inline
#   ifdef __GNUC__
#       define force_inline __attribute__((always_inline))
#   else
#       define force_inline
#   endif
#endif

namespace jebe {

template<typename T>
class Memory
{
public:
    typedef T   type;
    typedef T*  pointer;

    Memory()
        : incr_step(1)
    {}

    explicit Memory(const std::size_t reserve, const std::size_t step = 1)
        : incr_step(step)
    {
        alloc(reserve);
    }

    pointer force_inline get(const std::size_t n)
    {
        return ((n == 1 || slices.begin()->second >= n) && !slices.empty())
            ? slices.begin()->first :  get_multi(n);
    }

    pointer inline get_multi(const std::size_t n)
    {
        for (typename SliceList::iterator iter = slices.begin(); iter != slices.end(); ++iter)
        {
            if (iter->second == n)
            {
                slices.erase(iter);
                return iter->first;
            }
            else if (iter->second > n)
            {
                iter->first += n;
                iter->second -= n;
                return iter->first;
            }
        }
        alloc(n > incr_step ? n : incr_step);
        return get_multi(n);
    }

    void force_inline revert(pointer p, const std::size_t n)
    {
        slices.push_back(std::make_pair(p, n));
    }

    void force_inline revert(void* p, const std::size_t s)
    {
        
    }

private:
    void force_inline alloc()
    {
        pointer p = static_cast<pointer>(malloc(sizeof(type)));
        slices.push_back(std::make_pair(p, 1));
    }

    void force_inline alloc(const std::size_t n)
    {
        pointer p = static_cast<pointer>(malloc(n * sizeof(type)));
        slices.push_back(std::make_pair(p, n));
    }

    const std::size_t incr_step;

    typedef typename std::pair<pointer, std::size_t> Slice;
    typedef typename std::list<Slice> SliceList;
    SliceList slices;
};

template<>
class Memory<char>
{
public:
    typedef char   type;
    typedef void*  pointer;

    explicit Memory()
        : incr_step(1)
    {}

    explicit Memory(const std::size_t reserve, const std::size_t step = 1)
        : incr_step(step)
    {
        alloc(reserve);
    }

    Memory(const Memory& m)
        : incr_step(m.incr_step), slices(m.slices)
    {}

    pointer force_inline get(const std::size_t n)
    {
        return ((n == 1 || slices.begin()->second >= n) && !slices.empty()) ?
            slices.begin()->first : get_multi(n);
    }

    inline pointer get_multi(const std::size_t n)
    {
        for (SliceList::iterator iter = slices.begin(); iter != slices.end(); ++iter)
        {
            if (iter->second == n)
            {
                slices.erase(iter);
                return iter->first;
            }
            else if (iter->second > n)
            {
                __extension__ iter->first += n;
                __extension__ iter->second -= n;
                return iter->first;
            }
        }
        return alloc(n > incr_step ? n : incr_step);
    }

    void force_inline revert(pointer p, const std::size_t n)
    {
        slices.push_back(std::make_pair(p, n));
    }

private:
    void force_inline alloc(const std::size_t n)
    {
        slices.push_back(std::make_pair(malloc(n * sizeof(type)), n));
    }

    const std::size_t incr_step;

    typedef std::pair<pointer, std::size_t> Slice;
    typedef std::list<Slice> SliceList;
    SliceList slices;
};

// for normal usage
static Memory<char> memory(1 << 17, 1 << 17);

template<typename T>
T* force_inline create()
{
    return new (memory.get(sizeof(T))) T();
}

template<typename T>
void force_inline destroy(T* p)
{
    p->~T();
    memory.revert(p, sizeof(T));
}

template<typename T, const std::size_t reserve = 1, const std::size_t incr_step = 1>
class Allocator
{
public:
    typedef std::size_t    size_type;
    typedef ptrdiff_t difference_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef T         value_type;

    typedef Memory<T> Chunk;

    template<typename TN>
    struct rebind
    {
        typedef Allocator<TN> other;
    };

    Allocator(const Allocator& a) throw()
        : chunk(a.chunk)
    {}

    template<typename TN>
    Allocator(const Allocator<TN>&) throw()
    {}

    Allocator() throw()
        : chunk(reserve, incr_step)
    {}

    pointer allocate(const std::size_t n, const void* = 0)
    {
        return static_cast<pointer>(chunk.get(n));
    }

    void deallocate(pointer p, const size_type n)
    {
        chunk.revert(p, n);
    }

    void deallocate(void* p, const size_type s)
    {
        chunk.revert(p, s);
    }

    void construct(pointer p, const value_type& val)
    {
        new (p) value_type(val);
    }

    pointer address(reference t) const
    {
        return &t;
    }

    const_pointer address(const_reference t) const
    {
        return &t;
    }

    size_type max_size() const throw()
    {
        return size_t(-1) / sizeof(value_type);
    }

    void destroy(pointer p)
    {
        p->~value_type();
    }

    ~Allocator() throw() {}

private:
    Chunk chunk;
};

template<typename T, const std::size_t reserve, const std::size_t incr_step>
class Creator
{
public:
    typedef T   type;
    typedef T*  pointer;
    typedef Allocator<T, reserve, incr_step> _Allocator;
    
    void* operator new(const std::size_t s, const void* = 0)
    {
        return allocator.allocate(1);
    }

    void operator delete(void* p, const std::size_t s)
    {
        allocator.deallocate(p, s);
    }

    void free()
    {
        allocator.deallocate(this, 1);
    }
    
protected:
    static _Allocator allocator;
};

template<typename T, const std::size_t reserve, const std::size_t incr_step>
Allocator<T, reserve, incr_step> Creator<T, reserve, incr_step>::allocator;

}
