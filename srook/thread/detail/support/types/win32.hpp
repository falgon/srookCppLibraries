// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_THREAD_DETAIL_SUPPORT_TYPES_WIN32_HPP
#define INCLUDED_SROOK_THREAD_DETAIL_SUPPORT_TYPES_WIN32_HPP

#include <srook/config/environment/os/thread_api.hpp>
#include <srook/config/feature/inline_namespace.hpp>

#if SROOK_HAS_THREADS
#    if SROOK_HAS_THREAD_API_WIN32

#	include <fibersapi.h>
#	include <process.h>
#   include <srook/config/compiler/includes/msvc/windows.h>

namespace srook {
namespace threading {
SROOK_INLINE_NAMESPACE(v1)
namespace detail {

typedef SRWLOCK mutex_type;
#        define SROOK_MUTEX_INITIALIZER SRWLOCK_INIT
typedef CRITICAL_SECTION recursive_mutex_type;
typedef CONDITION_VARIABLE cond_var_type;
#        define SROOK_COND_VAR_INITIALIZER CONDITION_VARIABLE_INIT
typedef INIT_ONCE exec_once_flag_type;
#        define SROOK_EXEC_ONCE_INITIALIZER INIT_ONCE_STATIC_INIT
typedef DWORD thread_id_type;
#        define SROOK_NULL_THREAD SROOK_NULLPTR
typedef HANDLE thread_type;
typedef DWORD tls_key_type;
#        define SROOK_TLS_DESTRUCTOR_CC WINAPI

} // namespace detail
SROOK_INLINE_NAMESPACE_END
} // namespace thread
} // namespace srook

#    endif
#    else
#        error not supported in this environment
#    endif
#endif
