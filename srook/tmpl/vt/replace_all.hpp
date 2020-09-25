// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_TMPL_VT_REPLACE_ALL_HPP
#define INCLUDED_SROOK_TMPL_VT_REPLACE_ALL_HPP

#include <srook/tmpl/vt/detail/config.hpp>

SROOK_NESTED_NAMESPACE(srook, tmpl, vt) {
SROOK_INLINE_NAMESPACE(v1)

template <class T, class... Ts>
struct replace_all 
    : public type_constant<SROOK_DEDUCED_TYPENAME detail::ReplaceAll<T, Ts...>::type::rebind_packer> {};

template <class T, class... Ts>
struct replace_all<T, packer<Ts...>> : public replace_all<T, Ts...> {};

#if SROOK_ALIAS_TEMPLATES
template <class T, class... Ts>
using replace_all_t = SROOK_DEDUCED_TYPENAME replace_all<T, Ts...>::type;
#endif

SROOK_INLINE_NAMESPACE_END
} SROOK_NESTED_NAMESPACE_END(vt, tmpl, srook)

#endif
