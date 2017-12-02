// Copyright (C) 2017 roki
#ifndef INCLUDED_SROOK_ARRAY_ARRAY_HPP
#define INCLUDED_SROOK_ARRAY_ARRAY_HPP

#include <srook/array/array_traits.hpp>
#include <srook/type_traits.hpp>
#include <srook/utility.hpp>
#include <algorithm>
#include <stdexcept>
#if 0 && SROOK_HAS_INCLUDE(<compare>)
#   include <compare>
#endif

namespace srook {
namespace array_ns {
SROOK_INLINE_NAMESPACE(v1)

template <class T, std::size_t N>
struct array {
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef array_traits<T, N> AT;
    SROOK_DEDUCED_TYPENAME AT::type elem_;

    // aggregate type...
    
    SROOK_CXX14_CONSTEXPR pointer data() SROOK_NOEXCEPT_TRUE { return AT::ptr(elem_); }
    SROOK_CXX14_CONSTEXPR const_pointer data() const SROOK_NOEXCEPT_TRUE { return AT::ptr(elem_); }
    SROOK_CXX14_CONSTEXPR iterator begin() SROOK_NOEXCEPT_TRUE { return iterator(data()); }
    SROOK_CXX14_CONSTEXPR const_iterator begin() const SROOK_NOEXCEPT_TRUE { return const_iterator(data()); }
    SROOK_CXX14_CONSTEXPR iterator end() SROOK_NOEXCEPT_TRUE { return iterator(data() + N); }
    SROOK_CXX14_CONSTEXPR const_iterator end() const SROOK_NOEXCEPT_TRUE { return const_iterator(data() + N); }
    SROOK_CXX14_CONSTEXPR reverse_iterator rbegin() SROOK_NOEXCEPT_TRUE { return reverse_iterator(end()); }
    SROOK_CXX14_CONSTEXPR const_reverse_iterator rbegin() const SROOK_NOEXCEPT_TRUE { return const_reverse_iterator(end()); }
    SROOK_CXX14_CONSTEXPR reverse_iterator rend() SROOK_NOEXCEPT_TRUE { return reverse_iterator(begin()); }
    SROOK_CXX14_CONSTEXPR const_reverse_iterator rend() const SROOK_NOEXCEPT_TRUE { return const_reverse_iterator(begin()); }
    SROOK_CXX14_CONSTEXPR const_iterator cbegin() const SROOK_NOEXCEPT_TRUE { return const_iterator(data()); }
    SROOK_CXX14_CONSTEXPR const_iterator cend() const SROOK_NOEXCEPT_TRUE { return const_iterator(data() + N); }
    SROOK_CXX14_CONSTEXPR const_iterator crbegin() const SROOK_NOEXCEPT_TRUE { return const_reverse_iterator(end()); }
    SROOK_CXX14_CONSTEXPR const_iterator crend() const SROOK_NOEXCEPT_TRUE { return const_reverse_iterator(begin()); }

    SROOK_CONSTEXPR size_type size() const SROOK_NOEXCEPT_TRUE { return N; }
    SROOK_CONSTEXPR size_type max_size() const SROOK_NOEXCEPT_TRUE { return size(); }
    SROOK_CONSTEXPR bool empty() const SROOK_NOEXCEPT_TRUE { return size() == 0; }

    SROOK_CONSTEXPR reference operator[](size_type n) SROOK_NOEXCEPT_TRUE { return AT::ref(elem_, n); }
    SROOK_CONSTEXPR const_reference operator[](size_type n) const SROOK_NOEXCEPT_TRUE { return AT::ref(elem_, n); }
    SROOK_CONSTEXPR reference at(size_type n)
    {
        return n >= N ? SROOK_THROW std::out_of_range("srook::array::at: index is out of range"), operator[](n) : operator[](n);
    }
    SROOK_CONSTEXPR const_reference at(size_type n) const
    {
        return n >= N ? SROOK_THROW std::out_of_range("srook::array::at: index is out of range"), operator[](n) : operator[](n);
    }
    SROOK_CONSTEXPR reference front() SROOK_NOEXCEPT_TRUE
    {
        return operator[](0);
    }
    SROOK_CONSTEXPR const_reference front() const SROOK_NOEXCEPT_TRUE
    {
        return operator[](0);
    }
    SROOK_CONSTEXPR reference back() SROOK_NOEXCEPT_TRUE
    {
        return N ? operator[](N - 1) : operator[](N);
    }
    SROOK_CONSTEXPR const_reference back() const SROOK_NOEXCEPT_TRUE
    {
        return N ? operator[](N - 1) : operator[](N);
    }
    void fill(const value_type& v)
    {
        std::fill_n(begin(), size(), v);
    }
    void swap(array& other) SROOK_NOEXCEPT(AT::is_nothrow_swappable)
    {
        std::swap_ranges(begin(), end(), other.begin());
    }
    friend void swap(array& lhs, array& rhs)
    SROOK_NOEXCEPT(lhs.swap(rhs))
    {
        lhs.swap(rhs);
    }
#if SROOK_CPP_STRUCTURED_BINDINGS
    template <std::size_t I>
    friend SROOK_CONSTEXPR decltype(auto) get(const array& this_) SROOK_NOEXCEPT_TRUE
    {
        SROOK_STATIC_ASSERT(I < N, "array index is within bounds");
        return array_traits<T, N>::ref(this_.elem_, I);
    }
#endif

#if 0 && SROOK_HAS_INCLUDE(<compare>)
    // common_comparison_category_t<AT::type> always returns void if AT::type is not copyable ([class.spaceship])
    // because the core language doesn't provide comparison if the array is not copyable in the language, to keep copying and comparison consistent.
    // p0515r2 2.2.3 Notes
    typedef conditional_t<
        is_copyable_v<SROOK_DEDUCED_TYPENAME AT::type>, 
        std::common_comparison_category_t<SROOK_DEDUCED_TYPENAME AT::type>,
        std::common_comparison_category_t<value_type>
    > category_type;
    
    SROOK_CONSTEXPR category_type operator<=>(const array& other)
    {
        // T[N] <=> T[N] should return the same type as T's <=>. 
        // p0515r2 2.2.3 
        SROOK_ST_ASSERT((is_same<category_type, std::common_comparison_category_t<value_type>>::value));
        return std::lexicographical_compare_3way(cbegin(), cend(), other.cbegin(), other.cend());
    }
#else
    friend inline bool operator==(const array& lhs, const array& rhs) { return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()); }
    friend inline bool operator!=(const array& lhs, const array& rhs) { return !(lhs == rhs); }
    friend inline bool operator<(const array& lhs, const array& rhs) { return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()); }
    friend inline bool operator>(const array& lhs, const array& rhs) { return rhs < lhs; }
    friend inline bool operator<=(const array& lhs, const array& rhs) { return !(lhs > rhs); }
    friend inline bool operator>=(const array& lhs, const array& rhs) { return !(lhs < rhs); }
#endif
};

#if SROOK_CPP_DEDUCTION_GUIDES
template <class T, class... Ts>
array(T, Ts...) 
-> array<SROOK_DEDUCED_TYPENAME enable_if<type_traits::detail::Land<is_same<T, Ts>...>::value, T>::type, sizeof...(Ts) + 1>; 
#endif

template <class T, class... Ts>
SROOK_CONSTEXPR SROOK_DEDUCED_TYPENAME 
enable_if<type_traits::detail::Land<is_same<SROOK_DEDUCED_TYPENAME decay<T>::type, SROOK_DEDUCED_TYPENAME decay<Ts>::type>...>::value, array<T, sizeof...(Ts) + 1> >::type
make_array(T&& t, Ts&&... ts) SROOK_NOEXCEPT_TRUE
{
    return array{srook::forward<T>(t), srook::forward<Ts>(ts)...};
}

SROOK_INLINE_NAMESPACE_END
} // namespace array_ns

using array_ns::array;
using array_ns::make_array;

} // namespace srook

#if SROOK_CPP_STRUCTURED_BINDINGS
namespace std {

template <class T, std::size_t N>
struct tuple_size<srook::array<T, N> > 
    : public srook::integral_constant<std::size_t, N> {};

template <std::size_t I, class T, std::size_t N>
struct tuple_element<I, srook::array<T, N> > 
    : public srook::type_constant<SROOK_DECLTYPE((srook::declval<srook::array<T, N>>()[I]))> {
    SROOK_STATIC_ASSERT(I < N, "index is out of bounds");
};

} // namespace std
#endif

#endif
