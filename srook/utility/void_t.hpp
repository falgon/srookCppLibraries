// Copyright (C) 2017 roki
#ifndef INCLUDED_SROOK_UTILITY_VOID_T_HPP
#define INCLUDED_SROOK_UTILITY_VOID_T_HPP
#include <srook/config/cpp_predefined/feature_testing.hpp>
#if SROOK_CPP_VARIADIC_TEMPLATES
#include <cstddef>

namespace srook {
namespace utility {
namespace detail {
inline namespace v1 {

template <class T, class... Ts>
struct Empty_type {
    constexpr std::size_t size() const noexcept { return sizeof...(Ts); }
    using type = T;
};

template <class... Ts>
struct void_t_impl : Empty_type<void, Ts...> {
};

} // namespace v1
} // namespace detail
} // namespace utility

#if SROOK_CPP_ALIAS_TEMPLATES

template <class... Ts>
using void_t = typename utility::detail::void_t_impl<Ts...>::type;

#endif

template <class... Ts>
struct voider {
    using type = typename utility::detail::void_t_impl<Ts...>::type;
};

} // namespace srook

#endif
#endif
