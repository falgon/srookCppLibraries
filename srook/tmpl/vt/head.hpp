// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_TMPL_VT_HEAD_HPP
#define INCLUDED_SROOK_TMPL_VT_HEAD_HPP

#ifdef _MSC_VER
#   if _MSC_VER > 1000
#       pragma once
#   endif
#endif

#include <srook/tmpl/vt/first.hpp>

SROOK_NESTED_NAMESPACE(srook, tmpl, vt) {
SROOK_INLINE_NAMESPACE(v1)

//template <class... Ts>
//struct head : first<Ts...> {};

template <class...>
struct head;

template <class X, class... Xs>
struct head<X, Xs...> : type_constant<X> {};

template <class... Xs>
struct head<packer<Xs...>> : head<Xs...> {};

#if SROOK_CPP_ALIAS_TEMPLATES
template <class... Ts>
using head_t = SROOK_DEDUCED_TYPENAME head<Ts...>::type;
#endif

SROOK_INLINE_NAMESPACE_END
} SROOK_NESTED_NAMESPACE_END(vt, tmpl, srook)

#endif
