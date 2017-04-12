#ifndef INCLUDED_SROOK_TUPLE_SPLIT_HPP
#define INCLUDED_SROOK_TUPLE_SPLIT_HPP
#include<tuple>
#include<type_traits>

namespace srook{

namespace detail{

template<class... Tpl,std::size_t... v>
constexpr auto split_first_impl(const std::tuple<Tpl...>& tp,std::index_sequence<v...>)
noexcept(noexcept(std::make_tuple(std::get<v>(tp)...)))
-> decltype(std::make_tuple(std::get<v>(tp)...))
{
	return std::make_tuple(std::get<v>(tp)...);
}

struct Applyer{
	struct invoker{
		template<class... Ts,std::size_t n,std::size_t index,class... Args>
		static constexpr auto
		apply(const std::tuple<Ts...>&,std::integral_constant<std::size_t,n>,std::integral_constant<std::size_t,index>,Args&&... args)
		{
			return std::make_tuple(std::forward<Args>(args)...);
		}
	};

	struct applyer{
		template<class... Ts,std::size_t n,std::size_t index,class... Args>
		static constexpr auto
		apply(const std::tuple<Ts...>& t,std::integral_constant<std::size_t,n>,std::integral_constant<std::size_t,index>,Args&&... args)
		{
			return Applyer::apply(t,std::integral_constant<std::size_t,n-1>(),std::integral_constant<std::size_t,index-1>(),std::get<index>(t),std::forward<Args>(args)...);
		}
	};

	template<class... Ts,std::size_t n,std::size_t index,class... Args>
	static constexpr auto 
	apply(const std::tuple<Ts...>& t,std::integral_constant<std::size_t,n> integral_cons,std::integral_constant<std::size_t,index> in,Args&&... args)
	{
		return std::conditional_t<
			n==0,
			invoker,
			applyer
		>::apply(t,std::move(integral_cons),std::move(in),std::forward<Args>(args)...);
	}
};

template<std::size_t index,class... Tpl>
constexpr auto split_last_impl(const std::tuple<Tpl...>& tpl)
noexcept(noexcept(Applyer::apply(tpl,std::declval<std::integral_constant<std::size_t,std::tuple_size<std::tuple<Tpl...>>::value-index>>(),std::declval<std::integral_constant<std::size_t,std::tuple_size<std::tuple<Tpl...>>::value-1>>())))
->decltype(Applyer::apply(tpl,std::declval<std::integral_constant<std::size_t,std::tuple_size<std::tuple<Tpl...>>::value-index>>(),std::declval<std::integral_constant<std::size_t,std::tuple_size<std::tuple<Tpl...>>::value-1>>()))
{
	return 
		Applyer::apply(
			tpl,
			std::integral_constant<std::size_t,std::tuple_size<std::tuple<Tpl...>>::value-index>(),
			std::integral_constant<std::size_t,std::tuple_size<std::tuple<Tpl...>>::value-1>()
		);
}



} // namespace detail

template<std::size_t index,class... Tpl>
constexpr auto tuple_split_first(const std::tuple<Tpl...>& tpl)
noexcept(noexcept(detail::split_first_impl(tpl,std::declval<std::make_index_sequence<index>>())))
->decltype(detail::split_first_impl(tpl,std::declval<std::make_index_sequence<index>>()))
{
	return detail::split_first_impl(tpl,std::make_index_sequence<index>());
}

template<std::size_t index,class... Tpl>
constexpr auto tuple_split_last(const std::tuple<Tpl...>& tpl)
noexcept(noexcept(detail::split_last_impl<index>(tpl)))
->decltype(detail::split_last_impl<index>(tpl))
{
	return detail::split_last_impl<index>(tpl);
}


template<std::size_t index,class... Tpl>
constexpr auto tuple_split(const std::tuple<Tpl...>& tp)
noexcept(noexcept(std::make_pair(tuple_split_first<index>(tp),tuple_split_last<index>(tp))))
-> decltype(std::make_pair(tuple_split_first<index>(tp),tuple_split_last<index>(tp)))
{
	return std::make_pair(tuple_split_first<index>(tp),tuple_split_last<index>(tp));
}

} // namespace srook

#endif