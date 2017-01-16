#ifndef SROOK_INCLUDED_ADAPTOR_INSERT
#define SROOK_INCLUDED_ADAPTOR_INSERT
#include<tuple>
#include<iterator>
#include<iostream>
#include<boost/range/algorithm/copy.hpp>
#include<srook/iterator/range_iterator.hpp>
#include<srook/range/adaptor/adaptor_operator.hpp>

namespace srook{
namespace adaptors{
inline namespace v1{

template<class Pred,class Range,class Tuple,std::size_t... I>
auto apply_(Pred pred,Range& r,Tuple& tpl,std::index_sequence<I...>)
{
	return pred(r,std::get<I>(tpl)...);
}

template<class Pred,class Range,class... Args,
	class Indices=std::make_index_sequence<std::tuple_size<std::tuple<Args...>>::value>>
auto apply(Pred pred,Range& r,std::tuple<Args...>& tpl)
{
	return apply_(pred,r,tpl,Indices());
}

template<class Predicate,class... Args>
struct variadicer_t{
	explicit variadicer_t(Predicate pred,Args... args)
		:pred_(pred),args_(args...){}
	template<class Range>
	auto operator()(Range& r){return apply(pred_,r,args_);}
private:
	Predicate pred_;
	std::tuple<Args...> args_;
};

template<class Predicate,class... Args>
variadicer_t<Predicate,Args...> variadic(Predicate pred,Args&&... args)
{
	return variadicer_t<Predicate,Args...>(pred,std::forward<Args>(args)...);
}

const struct emplacer_t{
	template<class Range,class... Args>
	Range& operator()(Range& r,Args&&... args)
	{
		(r.emplace_back(std::forward<Args>(args)),...);
		return r;
	}
}emplacer={};

const struct emplace_fronter_t{
	template<class Range,class... Args>
	Range& operator()(Range& r,Args&&... args)
	{
		(r.emplace_front(std::forward<Args>(args)),...);
		return r;
	}
}emplace_fronter={};

} // inline namespace v1
} // namespace adaptors 
} // namespace srook
#endif
