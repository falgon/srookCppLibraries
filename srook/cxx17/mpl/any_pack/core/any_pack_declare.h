// Copyright (C) 2017 roki
#ifndef INCLUDED_SROOK_CXX17_MPL_ANY_PACK_DECLARE_H
#define INCLUDED_SROOK_CXX17_MPL_ANY_PACK_DECLARE_H

namespace srook{
namespace vmpl {
inline namespace mpl{
inline namespace v1{

template<auto...> struct any_pack;

} // inline namespace v1
} // inlnie namespace mpl 
} // namespace vmpl

using vmpl::any_pack;

} // namespace srook

#endif
