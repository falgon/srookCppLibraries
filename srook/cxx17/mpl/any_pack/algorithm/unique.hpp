// Copyright (C) 2017 roki
#ifndef INCLUDED_SROOK_CXX17_MPL_ANY_PACK_UNIQUE_HPP
#define INCLUDED_SROOK_CXX17_MPL_ANY_PACK_UNIQUE_HPP
#include<srook/mpl/variadic_player.hpp>
#include<srook/cxx17/mpl/any_pack/core/any_pack_declare.h>
#include<srook/cxx17/mpl/any_pack/algorithm/concat.hpp>

namespace srook{
inline namespace mpl{
inline namespace v1{
namespace detail{

template<class>
struct unique;
template<auto head,auto... tail>
struct unique<any_pack<head,tail...>>{
	using type=concat_t<any_pack<head>,typename unique<erase_all_elements_t<head,tail...>>::type>;
};
template<>
struct unique<any_pack<>>{
	using type=any_pack<>;
};
template<auto... v>
using unique_t=typename unique<any_pack<v...>>::type;

} // namespace detail
} // inline namespace v1
} // inlnie namespace mpl 
} // namespace srook

#endif
