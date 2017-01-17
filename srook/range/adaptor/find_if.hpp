#ifndef INLCLUDED_SROOK_RANGE_ADAPTOR_FIND_IF_HPP
#define INLCLUDED_SROOK_RANGE_ADAPTOR_FIND_IF_HPP
#include<utility>
#include<srook/range/adaptor/adaptor_operator.hpp>
#if __has_include(<boost/range/algorithm/find_if.hpp>)
#define POSSIBLE_TO_INCLUDE_BOOST_FIND_IF
#include<boost/range/algorithm/find_if.hpp>
#elif
#include<algorithm>
#endif
namespace srook{
namespace adaptors{
template<class Predicate>
struct find_if_t{
	explicit constexpr find_if_t(const Predicate& pred):pred_(pred){}
	explicit constexpr find_if_t(Predicate&& pred):pred_(std::move(pred)){}
	template<class Range>
	typename std::decay_t<Range>::const_iterator operator()(Range&& r)
	{
#ifdef POSSIBLE_TO_INCLUDE_BOOST_FIND_IF
		return boost::range::find_if(std::forward<Range>(r),std::move(pred_));
#elif
		return std::find_if(r.cbegin(),r.cend(),std::move(pred_));
#endif
	}
private:
	Predicate pred_;
};
template<class Predicate>
find_if_t<std::decay_t<Predicate>> find_if(Predicate&& pred)
{
	return find_if_t<std::decay_t<Predicate>>(std::forward<Predicate>(pred));
}

} // namespace adaptors
} // namespace srook

#ifdef POSSIBLE_TO_INCLUDE_BOOST_FIND_IF
#undef POSSIBLE_TO_INCLUDE_BOOST_FIND_IF
#endif
#endif