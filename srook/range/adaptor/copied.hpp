// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef SROOK_INCLUDED_RANGE_ADAPTOR_COPIED
#define SROOK_INCLUDED_RANGE_ADAPTOR_COPIED
#include <srook/range/adaptor/adaptor_operator.hpp>
#include <srook/iterator/range_iterators/range_iterator.hpp>
#include <srook/iterator/range_access.hpp>
#include <srook/type_traits/iterator/is_iterator.hpp>
#include <srook/cxx20/concepts/iterator/Iterator.hpp>
SROOK_NESTED_NAMESPACE(srook, range, adaptors) {
SROOK_INLINE_NAMESPACE(v1)

namespace detail {

template <class Iterator>
class copied_t : srook::iterator_traits<Iterator> {
    SROOK_STATIC_ASSERT(is_iterator<Iterator>::value, "Type must be iterator");
public:
    SROOK_FORCE_INLINE SROOK_CONSTEXPR copied_t(Iterator first, Iterator last) SROOK_NOEXCEPT(is_nothrow_constructible<Iterator&, Iterator>::value)
        : first_(first), last_(last) {}

    template <class Range, SROOK_REQUIRES(is_range<Range>::value)>
    SROOK_FORCE_INLINE SROOK_CONSTEXPR operator Range() 
    const SROOK_NOEXCEPT(is_nothrow_constructible<Range, Iterator, Iterator>::value)
    {
        return Range(first_, last_);
    }
private:
    const Iterator first_, last_;
};

struct copied_forward {
    template <class Range, SROOK_REQUIRES(is_range<SROOK_DEDUCED_TYPENAME decay<Range>::type>::value)>
    SROOK_FORCE_INLINE SROOK_CONSTEXPR copied_t<SROOK_DEDUCED_TYPENAME decay<Range>::type::iterator> 
    operator()(Range&& range) const
    SROOK_NOEXCEPT(
        is_nothrow_constructible<
            copied_t<SROOK_DEDUCED_TYPENAME decay<Range>::type::iterator>, 
            SROOK_DEDUCED_TYPENAME decay<Range>::type::iterator, 
            SROOK_DEDUCED_TYPENAME decay<Range>::type::iterator
        >::value
    )
    {
        return copied_t<SROOK_DEDUCED_TYPENAME decay<Range>::type::iterator>(srook::begin(range), srook::end(range));
    }
};

} // namespace detail

SROOK_INLINE_VARIABLE SROOK_CONSTEXPR_OR_CONST detail::copied_forward copied = {};

SROOK_INLINE_NAMESPACE_END
} SROOK_NESTED_NAMESPACE_END(adaptors, range, srook)

#endif
