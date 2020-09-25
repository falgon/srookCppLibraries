// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_CONFIG_EXTENSION_NULLABLE_HPP
#define INCLUDED_SROOK_CONFIG_EXTENSION_NULLABLE_HPP

#if defined(__clang__)
#	define SROOK_NULLABLE _Nullable
#	define SROOK_ATTRIBUTE_NULLABLE SROOK_NULLABLE
#else
#	define SROOK_NULLABLE
#	define SROOK_ATTRIBUTE_NULLABLE SROOK_NULLABLE
#endif

#endif
