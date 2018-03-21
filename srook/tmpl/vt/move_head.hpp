// Copyright (C) 2011-2018 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_TMPL_VT_MOVE_HEAD_HPP
#define INCLUDED_SROOK_TMPL_VT_MOVE_HEAD_HPP

#ifdef _MSC_VER
#   if _MSC_VER > 1000
#       pragma once
#   endif
#endif

#include <srook/tmpl/vt/detail/config.hpp>
#include <srook/tmpl/vt/at.hpp>
#include <srook/tmpl/vt/partial_head.hpp>
#include <srook/tmpl/vt/partial_tail.hpp>
#include <srook/tmpl/vt/concat.hpp>
#include <srook/type_traits/conditional.hpp>

SROOK_NESTED_NAMESPACE(srook, tmpl, vt) {
SROOK_INLINE_NAMESPACE(v1)

namespace detail {

template <std::size_t x, class... Xs>
struct move_head_impl
    : concat<
        SROOK_DEDUCED_TYPENAME at<x, Xs...>::type,
        SROOK_DEDUCED_TYPENAME concat<
            SROOK_DEDUCED_TYPENAME partial_head<x, Xs...>::type,
            SROOK_DEDUCED_TYPENAME partial_tail<x, Xs...>::type
        >::type
      > {};

template <class... Xs>
struct move_head_impl<0, Xs...>
    : type_constant<packer<Xs...>> {};

} // namespace detail

template <std::size_t x, class... Xs>
struct move_head : detail::move_head_impl<x, Xs...> {};

template <std::size_t x, class... Xs>
struct move_head<x, packer<Xs...>>
    : move_head<x, Xs...> {};

#if SROOK_CPP_ALIAS_TEMPLATES
template <std::size_t x, class... Xs>
using move_head_t = SROOK_DEDUCED_TYPENAME move_head<x, Xs...>::type;
#endif

SROOK_INLINE_NAMESPACE_END
} SROOK_NESTED_NAMESPACE_END(vt, tmpl, srook)

#endif
