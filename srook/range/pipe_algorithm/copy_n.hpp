// Copyright (C) 2017 roki
#ifndef INCLUDED_SROOK_ADAPTOR_COPY_N
#define INCLUDED_SROOK_ADAPTOR_COPY_N
#include<srook/range/adaptor/adaptor_operator.hpp>
#include<srook/type_traits/is_callable.hpp>
#include<srook/type_traits/has_iterator.hpp>
#include<srook/config/require.hpp>
#include<srook/iterator/range_iterator.hpp>
#if __has_include(<boost/range/algorithm/copy_n.hpp>)
#define POSSIBLE_TO_BOOST_RANGE_COPY_N
#include<boost/range/algorithm/copy_n.hpp>
#else
#include<algorithm>
#endif
namespace srook{
namespace pipealgo{
namespace detail{
inline namespace v1{

template<class Iterator>
struct copy_n_t{
	explicit constexpr copy_n_t(std::size_t n,Iterator iter)
		:n_(std::move(n)),iter_(std::move(iter)){}
	template<class Range,REQUIRES(has_iterator_v<std::decay_t<Range>> || is_range_iterator_v<std::decay_t<Range>>)>
	auto operator()(Range&& r)
	{
#ifdef POSSIBLE_TO_BOOST_RANGE_COPY_N
		boost::copy_n(r,std::move(n_),std::move(iter_));
#else
		std::copy_n(r.begin(),std::move(n_),std::move(iter_));
#endif
		return make_range_iterator(r.begin(),r.end());
	}
private:
	std::size_t n_;
	Iterator iter_;
};
template<class Iterator>
copy_n_t<std::decay_t<Iterator>> copy_n(std::size_t n,Iterator&& iter)
{
	return copy_n_t<std::decay_t<Iterator>>(std::move(n),std::forward<Iterator>(iter));
}

} // inline namespace v1
} // namespace detail

using detail::copy_n;

} // namespace pipealgo
} // namespace srook
#ifdef POSSIBLE_TO_BOOST_RANGE_COPY_N
#undef POSSIBLE_TO_BOOST_RANGE_COPY_N
#endif
#endif
