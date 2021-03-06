// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_CXX17_MPL_ANY_PACK_RANDOM_MAKER_HPP
#define INCLUDED_SROOK_CXX17_MPL_ANY_PACK_RANDOM_MAKER_HPP

#include<srook/cxx17/mpl/any_pack/core/any_pack_declare.h>

namespace srook{
namespace vmpl {
inline namespace mpl{
namespace random{
inline namespace v1{

template<std::size_t,class,class>
struct make_random_sequence_impl;

template<std::size_t N,class RandomEngine,auto... v>
struct make_random_sequence_impl<N,RandomEngine,any_pack<v...>>{
	using type = 
		typename make_random_sequence_impl<
			N - 1,
			typename RandomEngine::next_type,
  			any_pack<v...,RandomEngine::result>
		>::type;
};

template<class RandomEngine,auto... v>
struct make_random_sequence_impl<0,RandomEngine,any_pack<v...>>{
	using type = any_pack<v...>;
};

template<std::size_t N,class RandomEngine,class AnyPack>
using make_random_sequence = typename make_random_sequence_impl<N,RandomEngine,AnyPack>::type;

} // namespace v1
} // namespace random
} // inline namespace mpl
} // namespace vmpl
} // namespace srook

#endif
