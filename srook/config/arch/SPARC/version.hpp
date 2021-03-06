// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_CONFIG_ARCH_SPARC_VERSION_HPP
#define INCLUDED_SROOK_CONFIG_ARCH_SPARC_VERSION_HPP
#if SROOK_ARCH_IS_SPARC
#	if SROOK_ARCH_SPARCV9
#		define SROOK_ARCH_SPARC_VERSION SROOK_ARCH_SPARCV9
#	elif SROOK_ARCH_SPARCV8
#		define SROOK_ARCH_SPARC_VERSION SROOK_ARCH_SPARCV8
#	else
#		define SROOK_ARCH_SPARC_VERSION 0
#	endif
#endif
#endif
