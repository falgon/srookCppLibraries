// Copyright (C) 2017 roki
#ifndef INCLUDED_SROOK_MPL_TYPETRAITS_ADD_VOLATILE_HPP
#define INCLUDED_SROOK_MPL_TYPETRAITS_ADD_VOLATILE_HPP
#include <srook/config/cpp_predefined/feature_testing.hpp>
#include <srook/config/feature/inline_namespace.hpp>

namespace srook {
namespace type_traits {
SROOK_INLINE_NAMESPACE(v1)
namespace detail {

template <class T>
struct add_volatile {
    typedef volatile T type;
};

} // namespace detail
SROOK_INLINE_NAMESPACE_END
} // namespace type_traits

using type_traits::detail::add_volatile;

#if SROOK_CPP_ALIAS_TEMPLATES

template <class T>
using add_volatile_t = typename type_traits::detail::add_volatile<T>::type;

#endif

} // namespace srook
#endif