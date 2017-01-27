#ifndef INCLUDE_SROOK_RANGE_ADAPTOR_INPLACE_MERGE_HPP
#define INCLUDE_SROOK_RANGE_ADAPTOR_INPLACE_MERGE_HPP
#include<srook/range/adaptor/adaptor_operator.hpp>
#include<srook/config/require.hpp>
#include<srook/mpl/has_iterator.hpp>
#include<algorithm>

namespace srook{
namespace adaptors{
namespace detail{
inline namespace v1{

template<class Iterator>
struct inplace_iterator_t{
	explicit constexpr inplace_iterator_t(Iterator first,Iterator last):first_(std::move(first)),last_(std::move(last)){}
	
	template<class R>
	std::decay_t<R>& operator()(R&& range)
	{
		std::inplace_merge(range.begin(),std::move(first_),std::move(last_));
		return range;
	}
private:
	Iterator first_,last_;
};

template<class Iterator,class Compare>
struct inplace_merge_iterator_compare_t{
	explicit constexpr inplace_merge_iterator_compare_t(Iterator first,Iterator last,Compare comp):first_(std::move(first)),last_(std::move(last)),comp_(std::move(comp)){}
	template<class R>
	std::decay_t<R>& operator()(R&& range)
	{
		std::inplace_merge(range.begin(),std::move(first_),std::move(last_),std::move(comp_));
		return range;
	}
private:
	Iterator first_,last_;
	Compare comp_;
};

template<class Iterator,REQUIRES(!srook::mpl::has_iterator_v<std::decay_t<Iterator>>)>
constexpr inplace_iterator_t<std::decay_t<Iterator>> inplace_merge(Iterator&& first,Iterator&& last)
{
	return inplace_iterator_t<std::decay_t<Iterator>>(std::forward<Iterator>(first),std::forward<Iterator>(last));
}
template<class Iterator,class Compare>
constexpr inplace_merge_iterator_compare_t<std::decay_t<Iterator>,std::decay_t<Compare>> inplace_merge(Iterator&& first,Iterator&& last,Compare&& comp)
{
	return inplace_merge_iterator_compare_t<
		std::decay_t<Iterator>,
		std::decay_t<Compare>
	>(std::forward<Iterator>(first),std::forward<Iterator>(last),std::forward<Compare>(comp));
}

} // inline namespace v1
} // namespace detail

using detail::inplace_merge;

} // namespace adaptors
} // namespace srook

#endif