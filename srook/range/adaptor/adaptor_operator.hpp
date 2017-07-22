// Copyright (C) 2017 roki
#ifndef SROOK_INCLUDED_ADAPTOR_OPERATOR
#define SROOK_INCLUDED_ADAPTOR_OPERATOR
#include<utility>
namespace srook{
namespace adaptors{
namespace detail{
inline namespace v1{

template<class Range,class Function>
auto operator|(Range&& r,Function&& f)
{
	return std::forward<Function>(f)(std::forward<Range>(r));
}

} // inline namespace
} // namespace detail
} // namespace adaptors

namespace pipealgo{
namespace detail{
inline namespace v1{

template<class Range,class Function>
auto operator|(Range&& r,Function&& f)
{
	return std::forward<Function>(f)(std::forward<Range>(r));
}

} // inline namespace v1
} // namespace algo
} // namespace detail

} // namespace roki
#endif
