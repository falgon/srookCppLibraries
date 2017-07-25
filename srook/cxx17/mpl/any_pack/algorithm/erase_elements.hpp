// Copyright (C) 2017 roki
#ifndef INCLUDED_SROOK_CXX17_MPL_ANY_PACK_ERASE_ELEMENTS_HPP
#define INCLUDED_SROOK_CXX17_MPL_ANY_PACK_ERASE_ELEMENTS_HPP
#include<srook/mpl/variadic_player.hpp>
#include<srook/cxx17/mpl/any_pack/core/any_pack_declare.h>
#include<srook/cxx17/mpl/any_pack/algorithm/concat.hpp>
#include<optional>

namespace srook{
inline namespace mpl{
inline namespace v1{
namespace detail{

template<auto,auto...>
struct erase_elements;
template<auto target,auto head,auto... tail>
struct erase_elements<target,head,tail...>{
	using type = concat_t<any_pack<head>,typename erase_elements<target,tail...>::type>;
};
template<auto target,auto... tail>
struct erase_elements<target,target,tail...>{
	using type = any_pack<tail...>;
};
template<auto target>
struct erase_elements<target>{
	using type = any_pack<>;
};
template<auto target,auto... v>
using erase_elements_t = typename erase_elements<target,v...>::type;

} // namespace detail
} // inline namespace v1
} // inlnie namespace mpl 
} // namespace srook

#endif