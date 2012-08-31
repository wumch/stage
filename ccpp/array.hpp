
#pragma once

#include <iterator>

namespace staging {

template<class T, std::size_t N>
class Array
{
public:
    T elems[N]; // fixed-size array of elements of type T

public:
    // type definitions
    typedef T value_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // iterator support

    iterator
    begin()
    {
        return elems;
    }

    const_iterator
    begin() const
    {
        return elems;
    }

    const_iterator
    cbegin() const
    {
        return elems;
    }

    iterator
    end()
    {
        return elems + N;
    }

    const_iterator
    end() const
    {
        return elems + N;
    }

    const_iterator
    cend() const
    {
        return elems + N;
    }

    // reverse iterator support
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(BOOST_MSVC_STD_ITERATOR) && !defined(BOOST_NO_STD_ITERATOR_TRAITS)
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
#elif defined(_MSC_VER) && (_MSC_VER == 1300) && defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB == 310)
    // workaround for broken reverse_iterator in VC7
    typedef std::reverse_iterator<std::_Ptrit<value_type, difference_type, iterator,
    reference, iterator, reference> > reverse_iterator;
    typedef std::reverse_iterator<std::_Ptrit<value_type, difference_type, const_iterator,
    const_reference, iterator, reference> > const_reverse_iterator;
#elif defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)
    typedef std::reverse_iterator<iterator, std::random_access_iterator_tag,
    value_type, reference, iterator, difference_type> reverse_iterator;
    typedef std::reverse_iterator<const_iterator, std::random_access_iterator_tag,
    value_type, const_reference, const_iterator, difference_type> const_reverse_iterator;
#else
    // workaround for broken reverse_iterator implementations
    typedef std::reverse_iterator<iterator, T> reverse_iterator;
    typedef std::reverse_iterator<const_iterator, T> const_reverse_iterator;
#endif

    reverse_iterator
    rbegin()
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator
    rbegin() const
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator
    crbegin() const
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator
    rend()
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator
    rend() const
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator
    crend() const
    {
        return const_reverse_iterator(begin());
    }

    // operator[]

    reference operator[](size_type i)
    {
        return elems[i];
    }

    const_reference operator[](size_type i) const
    {
        return elems[i];
    }

    // at() with range check

    reference
    at(size_type i)
    {
        rangecheck(i);
        return elems[i];
    }

    const_reference
    at(size_type i) const
    {
        rangecheck(i);
        return elems[i];
    }

    // front() and back()

    reference
    front()
    {
        return elems[0];
    }

    const_reference
    front() const
    {
        return elems[0];
    }

    reference
    back()
    {
        return elems[N - 1];
    }

    const_reference
    back() const
    {
        return elems[N - 1];
    }

    // size is constant

    static size_type
    size()
    {
        return N;
    }

    static bool
    empty()
    {
        return false;
    }

    static size_type
    max_size()
    {
        return N;
    }

    enum
    {
        static_size = N
    };

    // swap (note: linear complexity)

    void
    swap(Array<T, N>& y)
    {
        for (size_type i = 0; i < N; ++i)
            boost::swap(elems[i], y.elems[i]);
    }

    // direct access to data (read-only)

    const T*
    data() const
    {
        return elems;
    }

    T*
    data()
    {
        return elems;
    }

    // use array as C array (direct read/write access to data)

    T*
    c_array()
    {
        return elems;
    }

    // assignment with type conversion

    template <typename T2>
            Array<T, N>& operator=(const Array<T2, N>& rhs)
    {
        std::copy(rhs.begin(), rhs.end(), begin());
        return *this;
    }

    // assign one value to all elements

    void
    assign(const T& value)
    {
        fill(value);
    } // A synonym for fill

    void
    fill(const T& value)
    {
        std::fill_n(begin(), size(), value);
    }

    // check range (may be private because it is static)

    static void
    rangecheck(size_type i)
    {
        if (i >= size())
        {
            std::out_of_range e("Array<>: index out of range");
            boost::throw_exception(e);
        }
    }

};

}
