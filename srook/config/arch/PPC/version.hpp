// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_CONFIG_PPC_VERSION_HPP
#define INCLUDED_SROOK_CONFIG_PPC_VERSION_HPP
#include <srook/config/arch/PPC/core.hpp>
#if SROOK_ARCH_IS_PPC
#	if SROOK_ARCH_PPC_440
#		define SROOK_ARCH_PPC_VERSION SROOK_ARCH_440
#	elif SROOK_ARCH_PPC_450
#		define SROOK_ARCH_PPC_VERSION SROOK_ARCH_450
#	elif SROOK_ARCH_PPC601
#		define SROOK_ARCH_PPC_VERSION SROOK_ARCH_PPC601
#	elif SROOK_ARCH_PPC603
#		define SROOK_ARCH_PPC_VERSION SROOK_ARCH_PPC603
#	elif SROOK_ARCH_PPC604
#		define SROOK_ARCH_PPC_VERSION SROOK_ARCH_PPC604
#	elif SROOK_ARCH_PPC620
#		define SROOK_ARCH_PPC_VERSION SROOK_ARCH_PPC620
#	else
#		define SROOK_ARCH_PPC_VERSION 0
#	endif
#else
#	define SROOK_ARCH_PPC_VERSION 0
#endif
#endif
