// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_CONFIG_EXTENSION_NULL_UNSPECIFIED_HPP
#define INCLUDED_SROOK_CONFIG_EXTENSION_NULL_UNSPECIFIED_HPP

#if defined(__clang__)
#	define SROOK_NULL_UNSPECIFIED _Null_unspecified
#	define SROOK_ATTRIBUTE_NULL_UNSPECIFIED SROOK_NULL_UNSPECIFIED
#else
#	define SROOK_NULL_UNSPECIFIED
#	define SROOK_ATTRIBUTE_NULL_UNSPECIFIED SROOK_NULL_UNSPECIFIED
#endif

#endif
