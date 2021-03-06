// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_CONFIG_FEATURE_STRONG_ENUM_HPP
#define INCLUDED_SROOK_CONFIG_FEATURE_STRONG_ENUM_HPP

#include <srook/config/cpp_predefined.hpp>
#include <srook/config/noexcept_detection.hpp>

#ifdef __clang__
#    define SROOK_STRONG_ENUM_BEGIN(x) _LIBCPP_DECLARE_STRONG_ENUM(x)
#    define SROOK_STRONG_ENUM_EPILOG(x) _LIBCPP_DECLARE_STRONG_ENUM_EPILOG(x)
#elif (SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT)
#    define SROOK_STRONG_ENUM_BEGIN(x) enum class x
#    define SROOK_STRONG_ENUM_EPILOG(x)
#else
#    define SROOK_STRONG_ENUM_BEGIN(x)              \
        struct x { \
            enum lx
#    define SROOK_STRONG_ENUM_EPILOG(x)                                            \
        lx v;                                                                      \
        SROOK_FORCE_INLINE x(lx v) : v_(v) {}                                      \
        SROOK_FORCE_INLINE explicit x(int v) : v_(static_cast<lx>(v)) {}           \
        SROOK_FORCE_INLINE operator int() const SROOK_NOEXCEPT_TRUE { return v_; } \
        }                                                                          \
        ;
#endif

#endif
