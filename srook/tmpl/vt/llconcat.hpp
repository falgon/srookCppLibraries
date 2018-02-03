// Copyright (C) 2011-2018 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_TMPL_VT_LLCONCAT_HPP
#define INCLUDED_SROOK_TMPL_VT_LLCONCAT_HPP

#include <srook/tmpl/vt/detail/config.hpp>

SROOK_NESTED_NAMESPACE(srook, tmpl, vt) {
SROOK_INLINE_NAMESPACE(v1)

template <class L, class... Rs>
struct llconcat : type_constant<packer<L, Rs...>> {};

#if SROOK_CPP_ALIAS_TEMPLATES
template <class... Ts>
using llconcat_t = SROOK_DEDUCED_TYPENAME llconcat<Ts...>::type;
#endif

SROOK_INLINE_NAMESPACE_END
} SROOK_NESTED_NAMESPACE_END(vt, tmpl, srook)

#endif
