// Copyright (C) 2017 roki
#ifndef INLCUDED_SROOK_PREPROCESSOR_CONCAT_HPP
#define INLCUDED_SROOK_PREPROCESSOR_CONCAT_HPP

#define SROOK_CONCAT1(X, Y) X##Y
#define SROOK_CONCAT(X, Y) SROOK_CONCAT1(X,Y)
#define SROOK_PP_CONCAT1(X, Y) SROOK_CONCAT1(X,Y)
#define SROOK_PP_CONCAT(X, Y) SROOK_PP_CONCAT1(X,Y)

#endif
