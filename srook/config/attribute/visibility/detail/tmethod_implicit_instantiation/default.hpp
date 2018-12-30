// Copyright (C) 2011-2019 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_CONFIG_ATTRIBUTE_VISIBILITY_DETAIL_TMETHOD_TEMPLATE_IMPLICIT_INSTANTIATION_DEFAULT_HPP
#define INCLUDED_SROOK_CONFIG_ATTRIBUTE_VISIBILITY_DETAIL_TMETHOD_TEMPLATE_IMPLICIT_INSTANTIATION_DEFAULT_HPP

#include <srook/config/user_config.hpp>
#include <srook/config/attribute/visibility/detail/function/hidden.hpp>

#if SROOK_CONFIG_DISABLE_VISIBILITY_ANNOTATIONS && !defined(SROOK_ATTRIBUTE_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS_DEFAULT)
#	define SROOK_ATTRIBUTE_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS_DEFAULT inline SROOK_ATTRIBUTE_FUNCTION_VIS_HIDDEN
#elif !defined(SROOK_ATTRIBUTE_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS_DEFAULT)
#	define SROOK_ATTRIBUTE_METHOD_TEMPLATE_IMPLICIT_INSTANTIATION_VIS_DEFAULT
#endif

#endif
