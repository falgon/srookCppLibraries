// Copyright (C) 2017 roki
#ifndef INCLUDED_SROOK_CXX17_MPL_ANY_PACK_AT_HPP
#define INCLUDED_SROOK_CXX17_MPL_ANY_PACK_AT_HPP
#include<srook/mpl/variadic_player.hpp>

namespace srook{
inline namespace mpl{
inline namespace v1{

namespace detail{

template<std::size_t,auto...>
struct at;
template<std::size_t index,auto head,auto... tail>
struct at<index,head,tail...>{
	static constexpr auto value=at<index-1,tail...>::value;
};
template<auto head,auto... tail>
struct at<0,head,tail...>{
	static constexpr decltype(head) value = head;
};
template<std::size_t target>
struct at<target>{
	static constexpr NULLOPT_T value = NULLOPT;
};

template<std::size_t n,auto... v>
constexpr std::conditional_t<!sizeof...(v),NULLOPT_T,At_t<n,decltype(v)...>> at_v = at<n,v...>::value;

} // namespace detail
} // inline namespace v1
} // inlnie namespace mpl 
} // namespace srook

#endif
