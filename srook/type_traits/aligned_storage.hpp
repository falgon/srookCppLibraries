// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_TYPE_TRAITS_ALIGNED_STORAGE_HPP
#define INCLUDED_SROOK_TYPE_TRAITS_ALIGNED_STORAGE_HPP

#include <srook/config/cpp_predefined.hpp>
#include <srook/config/feature.hpp>
#include <srook/type_traits/alignment_of.hpp>

namespace srook {
namespace type_traits {
SROOK_INLINE_NAMESPACE(v1)

namespace detail {

template <std::size_t L_>
struct aligned_storage_impl {
    union type_ {
        unsigned char data_[L_];
        struct SROOK_ATTRIBUTE_ALIGNED {} al_;
    };
};

} // namespace detail

template <std::size_t L_, std::size_t Al_
#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT
          = alignment_of<SROOK_DEDUCED_TYPENAME detail::aligned_storage_impl<L_>::type_>::value
#endif
          >
struct aligned_storage {
    union type {
        unsigned char data_[L_];
        struct
#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT
            SROOK_ATTRIBUTE_ALIGNED_X(Al_)
#else
            SROOK_ATTRIBUTE_ALIGNED_X(alignment_of<SROOK_DEDUCED_TYPENAME detail::aligned_storage_impl<L_>::type_>::value)
#endif
        {
        } al_;
    };
};

SROOK_INLINE_NAMESPACE_END
} // namespace type_traits

using type_traits::aligned_storage;

#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS14_CONSTANT && SROOK_CPP_ALIAS_TEMPLATES
template <std::size_t L, std::size_t Al = SROOK_ALIGN_OF(typename type_traits::detail::aligned_storage_impl<L>::type_)>
using aligned_storage_t = typename aligned_storage<L, Al>::type;
#endif

} // namespace srook
#endif
