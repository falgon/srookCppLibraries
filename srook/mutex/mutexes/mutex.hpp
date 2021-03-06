// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_MUTEX_MUTEX_HPP
#define INCLUDED_SROOK_MUTEX_MUTEX_HPP

#include <srook/config/cpp_predefined.hpp>
#include <srook/config/feature/exception.hpp>
#include <srook/mutex/includes/lib.hpp>
#include <srook/mutex/mutexes/detail/mutex_base.hpp>
#include <srook/type_traits/is_nothrow_default_constructible.hpp>
#include <srook/type_traits/library_concepts/is_mutex.hpp>

namespace srook {
namespace mutexes {
SROOK_INLINE_NAMESPACE(v1)

class SROOK_ATTRIBUTE_TYPE_VIS_DEFAULT
SROOK_THREAD_SAFETY_ANNOTATION(capability("mutex"))
    mutex : private mutex_base {
public:
    typedef native_type* native_handle_type;

    SROOK_MUTEX_DOES_NOT_SUPPORT_STATIC_INIT_MESSAGE
    SROOK_CONSTEXPR mutex() SROOK_NOEXCEPT_TRUE SROOK_DEFAULT

    ~mutex() SROOK_DEFAULT

    mutex(const mutex&) SROOK_NOEXCEPT_TRUE SROOK_EQ_DELETE

    mutex& operator=(const mutex&) SROOK_EQ_DELETE

    void lock() 
    SROOK_THREAD_SAFETY_ANNOTATION(acquire_capability())
    {
        const int e = threading::detail::mutex_lock(&m);
        if (e) includes::throw_system_err(e);
    }

    bool try_lock() 
    SROOK_NOEXCEPT_TRUE 
    SROOK_THREAD_SAFETY_ANNOTATION(try_acquire_capability(true))
    {
        return threading::detail::mutex_trylock(&m);
    }

    void unlock() 
    SROOK_NOEXCEPT_TRUE 
    SROOK_THREAD_SAFETY_ANNOTATION(release_capability())
    {
        threading::detail::mutex_unlock(&m);
    }

    SROOK_ATTRIBUTE_INLINE_VISIBILITY 
    native_handle_type native_handle() SROOK_NOEXCEPT_TRUE
    {
        return &m;
    }
};

#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT
SROOK_STATIC_ASSERT(is_mutex<mutex>::value&& is_nothrow_default_constructible<mutex>::value,
                    "the default constructor for srook::mutex must be nothrow");
#endif

SROOK_INLINE_NAMESPACE_END

} // namespace mutexes

using mutexes::mutex;

} // namespace srook

#endif
