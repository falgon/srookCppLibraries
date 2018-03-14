// Copyright (C) 2011-2018 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_MATH_CONSTANTS_LN_HPP
#define INCLUDED_SROOK_MATH_CONSTANTS_LN_HPP

#include <srook/config.hpp>

SROOK_NESTED_NAMESPACE(srook, math) {
SROOK_INLINE_NAMESPACE(v1)

template <class T>
SROOK_FORCE_INLINE SROOK_CONSTEXPR T ln_ten()
{
    return static_cast<T>(2.30258509299404568401799145468436420760110148862877297603332790096757260967735248023599720508959829834196778404L);
}

template <class T>
SROOK_FORCE_INLINE SROOK_CONSTEXPR T ln_two()
{
    return static_cast<T>(0.693147180559945309417232121458176568075500134360255254120680009493393621969694715605863326996418687542001481021L);
}

SROOK_INLINE_NAMESPACE_END
} SROOK_NESTED_NAMESPACE_END(math, srook)
#endif
