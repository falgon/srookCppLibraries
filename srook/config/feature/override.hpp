// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_CONFIG_FEATURE_OVERRIDE_HPP
#define INCLUDED_SROOK_CONFIG_FEATURE_OVERRIDE_HPP

#include <srook/config/cpp_predefined.hpp>

#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT
#    define SROOK_OVERRIDE override
#else
#    define SROOK_OVERRIDE
#endif

#endif
