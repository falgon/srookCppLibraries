// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_CONFIG_ARCH_SPARC_CORE_HPP
#define INCLUDED_SROOK_CONFIG_ARCH_SPARC_CORE_HPP
#if defined(__sparc__) || defined(__sparc) || defined(__sparc64__)
#   ifdef __sparc64__
#       define SROOK_ARCH_IS_SPARC64 1
#   else
#	    define SROOK_ARCH_IS_SPARC 1
#   endif
#	if defined(__sparcv9)
#		define SROOK_ARCH_SPARCV9 __sparcv9
#	else
#		define SROOK_ARCH_SPARCV9 0
#	endif
#	if defined(__sparcv8)
#		define SROOK_ARCH_SPARCV8 __sparcv8
#	else
#		define SROOK_ARCH_SPARCV8 0
#	endif
#else
#   define SROOK_ARCH_IS_SPARC64 0
#	define SROOK_ARCH_IS_SPARC 0
#	define SROOK_ARCH_SPARCV8 0
#	define SROOK_ARCH_SPARCV9 0
#endif
#endif
