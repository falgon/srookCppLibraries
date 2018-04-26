// Copyright (C) 2011-2018 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_RANGE_ADAPTOR_DETAIL_DEDUCE_ITER_HPP
#define INCLUDED_SROOK_RANGE_ADAPTOR_DETAIL_DEDUCE_ITER_HPP
#include <srook/range/adaptor/adaptor_operator.hpp>
#include <srook/type_traits/decay.hpp>
#include <srook/type_traits/type_constant.hpp>

SROOK_NESTED_NAMESPACE(srook, range, adaptors) {
SROOK_INLINE_NAMESPACE(v1)

namespace detail {

template <class Range>
struct deduce_iter
    : type_constant<SROOK_DEDUCED_TYPENAME decay<Range>::type::const_iterator> {};

} // namespace detail

SROOK_INLINE_NAMESPACE_END
} SROOK_NESTED_NAMESPACE_END(adaptors, range, srook)

#endif
