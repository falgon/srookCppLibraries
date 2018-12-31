// Copyright (C) 2011-2019 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_TUPLE_REVERSE_HPP
#define INCLUDED_SROOK_TUPLE_REVERSE_HPP
#include <srook/mpl/constant_sequence/reverse_sequence.hpp>
#include <srook/mpl/variadic_player.hpp>
#include <srook/config/libraries/type_traits/index_sequence.hpp>
#include <tuple>

namespace srook {
namespace tuple {
inline namespace v1{
namespace detail {

template <class... Ts, std::size_t... n>
constexpr Transfer_t<std::tuple, Reverse_t<Ts...>>
tuple_reverse_impl(const std::tuple<Ts...> &tpl, SROOK_INDEX_SEQUENCE<n...>)
{
    return std::make_tuple(std::get<n>(tpl)...);
}

} // namespace detail

template <class... Ts>
constexpr Transfer_t<std::tuple, Reverse_t<Ts...>> reverse(const std::tuple<Ts...> &tpl)
{
    return detail::tuple_reverse_impl(tpl, make_reverse_sequence<std::tuple_size<std::tuple<Ts...>>::value>());
}

} // inline namespace v1
} // namespace tuple
} // namespace srook
#endif
