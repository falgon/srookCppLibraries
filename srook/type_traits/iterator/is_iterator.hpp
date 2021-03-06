// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_TYPE_TRAITS_IS_ITERATOR_HPP
#define INCLUDED_SROOK_TYPE_TRAITS_IS_ITERATOR_HPP
#ifdef _MSC_VER
#   if _MSC_VER > 1000
#       pragma once
#   endif
#endif

#include <srook/config.hpp>
#include <srook/type_traits/is_dereferenceable.hpp>
#include <srook/type_traits/is_copy_constructible.hpp>
#include <srook/type_traits/is_copy_assignable.hpp>
#include <srook/type_traits/is_destructible.hpp>
#include <srook/type_traits/is_swappable.hpp>
#include <srook/type_traits/is_lvalue_reference.hpp>
#include <srook/utility/declval.hpp>
#include <iterator>

SROOK_NESTED_NAMESPACE(srook, type_traits) {
SROOK_INLINE_NAMESPACE(v1)

namespace detail {

template <class T>
struct is_iterator_accessible_require {
private:
    template <class U>
    static SROOK_DECLTYPE(
        declval<SROOK_DEDUCED_TYPENAME std::iterator_traits<U>::value_type>(),
        declval<SROOK_DEDUCED_TYPENAME std::iterator_traits<U>::difference_type>(),
        declval<SROOK_DEDUCED_TYPENAME std::iterator_traits<U>::reference>(),
        declval<SROOK_DEDUCED_TYPENAME std::iterator_traits<U>::pointer>(),
        declval<SROOK_DEDUCED_TYPENAME std::iterator_traits<U>::iterator_category>(),
        SROOK_TRUE_TYPE()
    ) test(const U&);
    static SROOK_FALSE_TYPE test(...);        
public:
    typedef SROOK_DECLTYPE(test(declval<T>())) type;
};

template <class T>
struct preincrementable_to_ref {
private:
    template <class U>
    static SROOK_DEDUCED_TYPENAME is_same<SROOK_DECLTYPE(++declval<U&>()), U&>::type test(const U&);
    static SROOK_FALSE_TYPE test(...);
public:
    typedef SROOK_DECLTYPE(test(declval<T>())) type;
};

} // namespace detail

template <class T>
struct is_iterator
    : ::srook::type_traits::detail::Lor<
        is_pointer<T>,
        ::srook::type_traits::detail::Land<
            is_dereferenceable<T>, SROOK_DEDUCED_TYPENAME detail::preincrementable_to_ref<T>::type,
            is_copy_assignable<T>, is_destructible<T>,
            SROOK_DEDUCED_TYPENAME conditional<is_lvalue_reference<T>::value, is_swappable<T>, SROOK_TRUE_TYPE>::type,
            SROOK_DEDUCED_TYPENAME detail::is_iterator_accessible_require<T>::type
        >
      > {};

#if SROOK_CPP_VARIABLE_TEMPLATES
template <class T>
SROOK_CONSTEXPR bool is_iterator_v = is_iterator<T>::value;
#endif

SROOK_INLINE_NAMESPACE_END
} SROOK_NESTED_NAMESPACE_END(type_traits, srook)

namespace srook {

using type_traits::is_iterator;

} // namespace srook

#endif
