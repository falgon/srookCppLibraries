// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_MEMORY_ALIGNED_DETAIL_ALIGNED_ALLOC_POSIX_HPP
#define INCLUDED_SROOK_MEMORY_ALIGNED_DETAIL_ALIGNED_ALLOC_POSIX_HPP

#include <srook/config.hpp>
#include <srook/memory/align/detail/is_alignment.hpp>
extern "C" {
#include <stdlib.h>
}

SROOK_NESTED_NAMESPACE(srook, memory, alignment) {
SROOK_INLINE_NAMESPACE(v1)

SROOK_FORCE_INLINE void* aligned_alloc(std::size_t a, std::size_t size) SROOK_NOEXCEPT_TRUE
{
    assert(detail::is_alignment(a));
    if (a < sizeof(void*)) {
        a = sizeof(void*);
    }
    
    void* res = SROOK_NULLPTR;
    return ::posix_memalign(&res, a, size) ? SROOK_NULLPTR : res;
}

SROOK_FORCE_INLINE void aligned_free(void* p) SROOK_NOEXCEPT_TRUE
{
    ::free(p);
}

SROOK_INLINE_NAMESPACE_END
} SROOK_NESTED_NAMESPACE_END(alignment, memory, srook)
#endif
