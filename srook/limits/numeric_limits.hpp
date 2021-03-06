// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_LIMITS_NUMERIC_LIMITS_HPP
#define INCLUDED_SROOK_LIMITS_NUMERIC_LIMITS_HPP

#include <limits>
#include <srook/config/attribute/force_inline.hpp>
#include <srook/config/cpp_predefined/__cplusplus_constant.hpp>
#include <srook/config/cpp_predefined/feature_testing.hpp>
#include <srook/config/cpp_predefined/macro_names.hpp>
#include <srook/config/feature/constexpr.hpp>
#include <srook/config/inline_variable.hpp>
#include <srook/config/noexcept_detection.hpp>
#include <srook/math/config/builtin.hpp>

#if !(SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT)
#    include <cfloat>
#    include <climits>
#    include <cstdint>
#    if !defined(__FLT_DENORM_MIN__) || !defined(__LDBL_DENORM_MIN__) || !defined(__LDBL_DENORM_MIN__)
#        include <cmath>
#    endif
#endif

namespace srook {

#if !(SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT)

namespace detail {

template <typename T>
class numeric_limits_base {
    typedef std::numeric_limits<T> base_type;

public:
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool is_specialized = base_type::is_specialized;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST int digits = base_type::digits;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST int digits10 = base_type::digits10;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST int max_digits10 = base_type::max_digits10;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool is_signed = base_type::is_signed;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool is_integer = base_type::is_integer;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool is_exact = base_type::is_exact;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST int radix = base_type::radix;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST int min_exponent = base_type::min_exponent;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST int max_exponent = base_type::max_exponent;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST int min_exponent10 = base_type::min_exponent10;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST int max_exponent10 = base_type::max_exponent10;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool has_infinity = base_type::has_infinity;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool has_quiet_NaN = base_type::has_quiet_NaN;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool has_signaling_NaN = base_type::has_signaling_NaN;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST std::float_denorm_style has_denorm = base_type::has_denorm;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool has_denorm_loss = base_type::has_denorm_loss;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool is_iec559 = base_type::is_iec559;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool is_bounded = base_type::is_bounded;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool is_modulo = base_type::is_modulo;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool traps = base_type::traps;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST bool tinyness_before = base_type::tinyness_before;
    SROOK_INLINE_VARIABLE static SROOK_CONSTEXPR_OR_CONST std::float_round_style round_style = base_type::round_style;
};

template <typename, bool>
class numeric_limits_impl;

template <typename T>
class numeric_limits_impl<T, true> : public numeric_limits_base<T> {
    typedef std::numeric_limits<T> base_type;

public:
#    define DEFINE_MEMBER_FUNCTION(x)                                                \
        SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST T x() SROOK_NOEXCEPT_TRUE \
        {                                                                            \
            return base_type::x();                                                   \
        }
    DEFINE_MEMBER_FUNCTION(min)
    DEFINE_MEMBER_FUNCTION(max)
    DEFINE_MEMBER_FUNCTION(lowest)
    DEFINE_MEMBER_FUNCTION(epsilon)
    DEFINE_MEMBER_FUNCTION(round_error)
    DEFINE_MEMBER_FUNCTION(infinity)
    DEFINE_MEMBER_FUNCTION(quiet_NaN)
    DEFINE_MEMBER_FUNCTION(signaling_NaN)
    DEFINE_MEMBER_FUNCTION(denorm_min)
#    undef DEFINE_MEMBER_FUNCTION
};

template <typename T>
class numeric_limits_impl<T, false> : public numeric_limits_base<T> {
    typedef std::numeric_limits<T> base_type;

public:
#    define DEFINE_RETURN_T_FUNCTION(x)                                              \
        SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST T x() SROOK_NOEXCEPT_TRUE \
        {                                                                            \
            return T();                                                              \
        }
    DEFINE_RETURN_T_FUNCTION(min)
    DEFINE_RETURN_T_FUNCTION(max)
    DEFINE_RETURN_T_FUNCTION(lowest)
    DEFINE_RETURN_T_FUNCTION(epsilon)
    DEFINE_RETURN_T_FUNCTION(round_error)
    DEFINE_RETURN_T_FUNCTION(infinity)
    DEFINE_RETURN_T_FUNCTION(quiet_NaN)
    DEFINE_RETURN_T_FUNCTION(signaling_NaN)
    DEFINE_RETURN_T_FUNCTION(denorm_min)
#    undef RETURN_T_FUNCTION
};

} // namespace detail

template <typename T>
class numeric_limits : public detail::numeric_limits_impl<T, std::numeric_limits<T>::is_specialized> {};

template <typename T>
class numeric_limits<const T> : public numeric_limits<T> {};

template <typename T>
class numeric_limits<volatile T> : public numeric_limits<T> {};

template <typename T>
class numeric_limits<const volatile T> : public numeric_limits<T> {};

#    define SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(TYPE, MIN, MAX)                             \
        template <>                                                                                     \
        class numeric_limits<TYPE> : public detail::numeric_limits_base<TYPE> {                         \
        public:                                                                                         \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE min() SROOK_NOEXCEPT_TRUE           \
            {                                                                                           \
                return MIN;                                                                             \
            }                                                                                           \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE max() SROOK_NOEXCEPT_TRUE           \
            {                                                                                           \
                return MAX;                                                                             \
            }                                                                                           \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE lowest() SROOK_NOEXCEPT_TRUE        \
            {                                                                                           \
                return min();                                                                           \
            }                                                                                           \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE epsilon() SROOK_NOEXCEPT_TRUE       \
            {                                                                                           \
                return static_cast<TYPE>(0);                                                            \
            }                                                                                           \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE round_error() SROOK_NOEXCEPT_TRUE   \
            {                                                                                           \
                return static_cast<TYPE>(0);                                                            \
            }                                                                                           \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE infinity() SROOK_NOEXCEPT_TRUE      \
            {                                                                                           \
                return static_cast<TYPE>(0);                                                            \
            }                                                                                           \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE quiet_NaN() SROOK_NOEXCEPT_TRUE     \
            {                                                                                           \
                return static_cast<TYPE>(0);                                                            \
            }                                                                                           \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE signaling_NaN() SROOK_NOEXCEPT_TRUE \
            {                                                                                           \
                return static_cast<TYPE>(0);                                                            \
            }                                                                                           \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE denorm_min() SROOK_NOEXCEPT_TRUE    \
            {                                                                                           \
                return static_cast<TYPE>(0);                                                            \
            }                                                                                           \
        }

#    define SROOK_DEFINE_NUMERIC_LIMITS_FLOAT_SPECIALIZED(TYPE, MIN, MAX, EPS, RND, INF, QNAM, SNAN, DMIN) \
        template <>                                                                                        \
        class numeric_limits<TYPE> : public detail::numeric_limits_base<TYPE> {                            \
        public:                                                                                            \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE min() SROOK_NOEXCEPT_TRUE              \
            {                                                                                              \
                return MIN;                                                                                \
            }                                                                                              \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE max() SROOK_NOEXCEPT_TRUE              \
            {                                                                                              \
                return MAX;                                                                                \
            }                                                                                              \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE lowest() SROOK_NOEXCEPT_TRUE           \
            {                                                                                              \
                return -max();                                                                             \
            }                                                                                              \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE epsilon() SROOK_NOEXCEPT_TRUE          \
            {                                                                                              \
                return EPS;                                                                                \
            }                                                                                              \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE round_error() SROOK_NOEXCEPT_TRUE      \
            {                                                                                              \
                return RND;                                                                                \
            }                                                                                              \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE infinity() SROOK_NOEXCEPT_TRUE         \
            {                                                                                              \
                return INF;                                                                                \
            }                                                                                              \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE quiet_NaN() SROOK_NOEXCEPT_TRUE        \
            {                                                                                              \
                return QNAM;                                                                               \
            }                                                                                              \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE signaling_NaN() SROOK_NOEXCEPT_TRUE    \
            {                                                                                              \
                return SNAN;                                                                               \
            }                                                                                              \
            SROOK_FORCE_INLINE static SROOK_CONSTEXPR_OR_CONST TYPE denorm_min() SROOK_NOEXCEPT_TRUE       \
            {                                                                                              \
                return DMIN;                                                                               \
            }                                                                                              \
        }

SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(bool, false, true);
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(short, SHRT_MIN, SHRT_MAX);
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(int, INT_MIN, INT_MAX);
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(long, LONG_MIN, LONG_MAX);
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(long long, LLONG_MIN, LLONG_MAX);
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(unsigned short, static_cast<unsigned short>(0), USHRT_MAX);
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(unsigned int, 0u, UINT_MAX);
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(unsigned long, 0ul, ULONG_MAX);
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(unsigned long long, 0ull, ULLONG_MAX);
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(signed char, SCHAR_MIN, SCHAR_MAX);
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(unsigned char, static_cast<unsigned char>(0), UCHAR_MAX);
#    if SROOK_CPP_UNICODE_LITERALS
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(char16_t, static_cast<std::uint_least16_t>(numeric_limits<std::uint_least16_t>::min()), static_cast<std::uint_least16_t>(numeric_limits<std::uint_least16_t>::max()));
SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED(char32_t, static_cast<std::uint_least32_t>(numeric_limits<std::uint_least32_t>::min()), static_cast<std::uint_least32_t>(numeric_limits<std::uint_least16_t>::max()));
#    endif

#    if !defined(__FLT_DENORM_MIN__)
SROOK_DEFINE_NUMERIC_LIMITS_FLOAT_SPECIALIZED(float, FLT_MIN, FLT_MAX, FLT_EPSILON, 0.5f, INFINITY, NAN, NAN, FLT_MIN);
#    else
SROOK_DEFINE_NUMERIC_LIMITS_FLOAT_SPECIALIZED(float, FLT_MIN, FLT_MAX, FLT_EPSILON, 0.5f, __builtin_huge_valf(), __builtin_nanf(""), __builtin_nansf(""), __FLT_DENORM_MIN__);
#    endif
#    if !defined(__DBL_DENORM_MIN__)
SROOK_DEFINE_NUMERIC_LIMITS_FLOAT_SPECIALIZED(double, DBL_MIN, DBL_MAX, DBL_EPSILON, 0.5, INFINITY, NAN, NAN, DBL_MIN);
#    else
SROOK_DEFINE_NUMERIC_LIMITS_FLOAT_SPECIALIZED(double, DBL_MIN, DBL_MAX, DBL_EPSILON, 0.5, __builtin_huge_val(), __builtin_nan(""), __builtin_nans(""), __DBL_DENORM_MIN__);
#    endif
#    if !defined(__LDBL_DENORM_MIN__)
SROOK_DEFINE_NUMERIC_LIMITS_FLOAT_SPECIALIZED(long double, LDBL_MIN, LDBL_MAX, LDBL_EPSILON, 0.5l, INFINITY, NAN, NAN, LDBL_MIN);
#    else
SROOK_DEFINE_NUMERIC_LIMITS_FLOAT_SPECIALIZED(long double, LDBL_MIN, LDBL_MAX, LDBL_EPSILON, 0.5l, __builtin_huge_vall(), __builtin_nanl(""), __builtin_nansl(""), __LDBL_DENORM_MIN__);
#    endif

#   if defined(SROOK_HAS_INT128) && !defined(__STRICT_ANSI__)
template <>
struct numeric_limits<srook::int128_t> : numeric_limits<long long> {
    static SROOK_INLINE_VARIABLE SROOK_CONSTEXPR_OR_CONST int digits = CHAR_BIT * sizeof(srook::int128_t) - 1;
    static const int digits10 = 38;
private:
    struct s {
        SROOK_CONSTEXPR s(srook::uint64_t upper, srook::uint64_t lower) : value(lower + (srook::int128_t(upper) << 64)) {}
        SROOK_CONSTEXPR operator srook::int128_t() const { return value; }
        srook::int128_t value;
    };
public:
    static SROOK_CONSTEXPR srook::int128_t min()
    {
        return s(0x8000000000000000, 0x0000000000000000);
    }

    static SROOK_CONSTEXPR srook::int128_t max()
    {
        return s(0x7fffffffffffffff, 0xffffffffffffffff);
    }

    static SROOK_CONSTEXPR lowest()
    {
        return min();
    }
};

template <>
struct numeric_limits<srook::uint128_t> : numeric_limits<long long> {
    static SROOK_INLINE_VARIABLE SROOK_CONSTEXPR_OR_CONST int digits = CHAR_BIT * sizeof(srook::uint128_t);
    static const int digits10 = 38;
private:
    struct s {
        SROOK_CONSTEXPR s(srook::uint64_t upper, srook::uint64_t lower) : value(lower + (srook::uint128_t(upper) << 64)) {}
        SROOK_CONSTEXPR operator srook::uint128_t() const { return value; }
        srook::uint128_t value;
    };
public:
    static SROOK_CONSTEXPR srook::uint128_t min()
    {
        return 0;
    }

    static SROOK_CONSTEXPR srook::uint128_t max()
    {
        return s(0xffffffffffffffff, 0xffffffffffffffff);
    }

    static SROOK_CONSTEXPR lowest()
    {
        return min();
    }
};


#   endif

#    undef SROOK_DEFINE_NUMERIC_LIMITS_INTEGER_SPECIALIZED
#    undef SROOK_DEFINE_NUMERIC_LIMITS_FLOAT_SPECIALIZED

#else

template <typename T>
class numeric_limits : public std::numeric_limits<T> {};

#endif
} // namespace srook

#if 0
#if defined(SROOK_HAS_INT128) && !defined(__STRICT_ANSI__)
namespace std {

template <>
struct numeric_limits<srook::int128_t> : numeric_limits<long long> {
    static SROOK_INLINE_VARIABLE SROOK_CONSTEXPR_OR_CONST int digits = CHAR_BIT * sizeof(srook::int128_t) - 1;
    static const int digits10 = 38;
private:
    struct s {
        SROOK_CONSTEXPR s(srook::uint64_t upper, srook::uint64_t lower) : value(lower + (srook::int128_t(upper) << 64)) {}
        SROOK_CONSTEXPR operator srook::int128_t() const { return value; }
        srook::int128_t value;
    };
public:
    static SROOK_CONSTEXPR srook::int128_t min()
    {
        return s(0x8000000000000000, 0x0000000000000000);
    }

    static SROOK_CONSTEXPR srook::int128_t max()
    {
        return s(0x7fffffffffffffff, 0xffffffffffffffff);
    }

    static SROOK_CONSTEXPR lowest()
    {
        return min();
    }
};

template <>
struct numeric_limits<srook::uint128_t> : numeric_limits<long long> {
    static SROOK_INLINE_VARIABLE SROOK_CONSTEXPR_OR_CONST int digits = CHAR_BIT * sizeof(srook::uint128_t);
    static const int digits10 = 38;
private:
    struct s {
        SROOK_CONSTEXPR s(srook::uint64_t upper, srook::uint64_t lower) : value(lower + (srook::uint128_t(upper) << 64)) {}
        SROOK_CONSTEXPR operator srook::uint128_t() const { return value; }
        srook::uint128_t value;
    };
public:
    static SROOK_CONSTEXPR srook::uint128_t min()
    {
        return 0;
    }

    static SROOK_CONSTEXPR srook::uint128_t max()
    {
        return s(0xffffffffffffffff, 0xffffffffffffffff);
    }

    static SROOK_CONSTEXPR lowest()
    {
        return min();
    }
};
} // namespace std
#endif
#endif

#endif
