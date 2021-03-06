// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_ALGORITHM_EQUAL_HPP
#define INCLUDED_SROOK_ALGORITHM_EQUAL_HPP
#include <algorithm>
#include <srook/config/require.hpp>
#include <srook/config/cpp_predefined.hpp>
#include <srook/config/feature/constexpr.hpp>
#include <srook/mpl/variadic_player.hpp>
#include <srook/tuple/algorithm/filter_type.hpp>
#include <srook/type_traits/has_iterator.hpp>
#include <srook/type_traits/conditional.hpp>
#include <srook/utility/forward.hpp>
#include <tuple>
#include <type_traits>

#include <iostream>

namespace srook {
namespace algorithm {
SROOK_INLINE_NAMESPACE(v1)
namespace detail {

template <bool, class>
struct Apply_Equal;

template <class... Pack>
struct Apply_Equal<false, pack<Pack...>> {
    template <class... LTs, class... RTs, class BinaryPred>
    constexpr static bool apply(const std::tuple<LTs...> &, const std::tuple<RTs...> &, BinaryPred &&)
    {
	return false;
    }
};

template <class Head, class Second, class... Tail>
struct Apply_Equal<true, pack<Head, Second, Tail...>> {
    template <class T>
    using is_same = std::is_same<T, Head>;

    struct Judger {
	struct invoker {
	    template <class... LTs, class... RTs, class... SL, class... SR, std::size_t n, class BinaryPred>
	    constexpr static bool
	    apply(const std::tuple<LTs...> &, const std::tuple<RTs...> &, const std::tuple<SL...> &sl, const std::tuple<SR...> &sr, std::integral_constant<std::size_t, n>, BinaryPred &&pred)
	    {
		return Apply_Equal<true, pack<Second, Tail...>>::apply(sl, sr, std::forward<BinaryPred>(pred));
	    }
	};
	struct unpacker {
	    template <class... LTs, class... RTs, class... SL, class... SR, std::size_t n, class BinaryPred>
	    static bool
	    apply(const std::tuple<LTs...> &l, const std::tuple<RTs...> &r, const std::tuple<SL...> &sl, const std::tuple<SR...> &sr, std::integral_constant<std::size_t, n>, BinaryPred &&pred)
	    {
		if (!std::forward<BinaryPred>(pred)(std::get<n>(l), std::get<n>(r))) {
		    return false;
		} else {
		    return Apply_Equal<
			true,
			pack<Head, Second, Tail...>>::Judger::apply(l, r, sl, sr, std::integral_constant<std::size_t, n - 1>(), std::forward<BinaryPred>(pred));
		}
	    }
	};

	template <class... LTs, class... RTs, class... SL, class... SR, std::size_t n, class BinaryPred>
	static bool
	apply(const std::tuple<LTs...> &l, const std::tuple<RTs...> &r, const std::tuple<SL...> &sl, const std::tuple<SR...> &sr, std::integral_constant<std::size_t, n> inc, BinaryPred &&pred)
	{
	    return typename conditional<n == 0, invoker, unpacker>::type::apply(l, r, sl, sr, std::move(inc), std::forward<BinaryPred>(pred));
	}
    };

    template <class... LTs, class... RTs, class BinaryPred>
    static bool apply(const std::tuple<LTs...> &l, const std::tuple<RTs...> &r, BinaryPred &&pred)
    {
	return Judger::apply(
	    srook::tuple::filter_type<is_same>(l),
	    srook::tuple::filter_type<is_same>(r),
	    l, r,
	    std::integral_constant<std::size_t, std::tuple_size<decltype(srook::tuple::filter_type<is_same>(l))>::value - 1>(),
	    std::forward<BinaryPred>(pred));
    }
};

template <class Tail>
struct Apply_Equal<true, pack<Tail>> {
    template <class T>
    using is_same = std::is_same<T, Tail>;

    struct Judger {
	struct invoker {
	    template <class... LTs, class... RTs, std::size_t n, class BinaryPred>
	    constexpr static bool
	    apply(const std::tuple<LTs...> &l, const std::tuple<RTs...> &r, std::integral_constant<std::size_t, n>, BinaryPred &&pred)
	    {
		return std::forward<BinaryPred>(pred)(std::get<n>(l), std::get<n>(r));
	    }
	};

	struct unpacker {
	    template <class... LTs, class... RTs, std::size_t n, class BinaryPred>
	    constexpr static bool
	    apply(const std::tuple<LTs...> &l, const std::tuple<RTs...> &r, std::integral_constant<std::size_t, n>, BinaryPred &&pred)
	    {
		if (!std::forward<BinaryPred>(pred)(std::get<n>(l), std::get<n>(r))) {
		    return false;
		} else {
		    return Apply_Equal<true, pack<Tail>>::Judger::apply(l, r, std::integral_constant<std::size_t, n - 1>(), std::forward<BinaryPred>(pred));
		}
	    }
	};

	template <class... LTs, class... RTs, std::size_t n, class BinaryPred>
	constexpr static bool
	apply(const std::tuple<LTs...> &l, const std::tuple<RTs...> &r, std::integral_constant<std::size_t, n> ic, BinaryPred &&pred)
	{
	    return typename conditional<n == 0, invoker, unpacker>::type::apply(l, r, std::move(ic), std::forward<BinaryPred>(pred));
	}
    };

    template <class... LTs, class... RTs, class BinaryPred>
    constexpr static bool
    apply(const std::tuple<LTs...> &l, const std::tuple<RTs...> &r, BinaryPred &&pred)
    {
	return Judger::apply(
	    srook::tuple::filter_type<is_same>(l),
	    srook::tuple::filter_type<is_same>(r),
	    std::integral_constant<
		std::size_t,
		std::tuple_size<decltype(srook::tuple::filter_type<is_same>(l))>::value - 1>(),
	    std::forward<BinaryPred>(pred));
    }
};

template <class, class>
struct sequence_same;

template <bool, class, class>
struct sequence_same_impl;

template <class... L, class... R>
struct sequence_same_impl<false, pack<L...>, pack<R...>> : std::false_type {
};

template <>
struct sequence_same_impl<true, pack<>, pack<>> : std::true_type {
};

template <class LH, class... LT, class RH, class... RT>
struct sequence_same_impl<true, pack<LH, LT...>, pack<RH, RT...>> {
    static constexpr bool value = sequence_same_impl<std::is_same<LH, RH>::value, pack<LT...>, pack<RT...>>::value;
};

template <class LH, class... LT, class RH, class... RT>
struct sequence_same<pack<LH, LT...>, pack<RH, RT...>> {
    static constexpr bool value = sequence_same_impl<std::is_same<LH, RH>::value, pack<LT...>, pack<RT...>>::value;
};

template <class L, class R>
constexpr bool sequence_same_v = sequence_same<L, R>::value;

#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS14_CONSTANT
const auto same_pred = [](const auto &x, const auto &y) { return x == y; };
#else
struct same_pred_t {
	template <class L, class R>
	SROOK_CONSTEXPR bool operator()(L&& l, R&& r) const SROOK_NOEXCEPT_TRUE
	{
		return srook::forward<L>(l) == srook::forward<R>(r);
	}
};
#endif

} // namespace detail

template <class... LTs, class... RTs, class BinaryPred = 
#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS14_CONSTANT
decltype(detail::same_pred)
#else
detail::same_pred_t
#endif
>
constexpr bool equal(const std::tuple<LTs...> &l, const std::tuple<RTs...> &r, const BinaryPred &pred = 
		detail::
#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS14_CONSTANT
		same_pred
#else
		same_pred_t()
#endif
		)
{
    return detail::Apply_Equal<
		       std::tuple_size<std::tuple<LTs...>>::value ==
		   std::tuple_size<std::tuple<RTs...>>::value and
	       detail::sequence_same_v<pack<LTs...>, pack<RTs...>>,
	   NoDuplicate_t<LTs...>>::apply(l, r, pred);
}

template <class L, class R, class BinaryPred = 
#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS14_CONSTANT
	decltype(detail::same_pred)
#else
	detail::same_pred_t
#endif
, REQUIRES(has_iterator<typename decay<L>::type>::value and has_iterator<typename decay<R>::type>::value)>
constexpr bool equal(L &&l, R &&r, const BinaryPred &pred = 
		detail::
#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS14_CONSTANT
		same_pred
#else
		same_pred_t()
#endif
		)
{
    return std::equal(std::begin(l), std::end(l), std::begin(r), std::end(r), pred);
}

template <class LInputIterator, class RInputIterator, class BinaryPred = 
#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS14_CONSTANT
	decltype(detail::same_pred)
#else
	detail::same_pred_t
#endif
	, REQUIRES(!has_iterator<typename decay<LInputIterator>::type>::value and !has_iterator<typename decay<RInputIterator>::type>::value)>
constexpr bool equal(LInputIterator &&lbegin, LInputIterator lend, RInputIterator &&rbegin, RInputIterator &&rend, const BinaryPred &pred = 
		detail::
#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS14_CONSTANT
		same_pred
#else
		same_pred_t()
#endif
		)
{
    return std::equal(std::forward<LInputIterator>(lbegin), std::forward<LInputIterator>(lend), std::forward<RInputIterator>(rbegin), std::forward<RInputIterator>(rend), pred);
}
SROOK_INLINE_NAMESPACE_END
} // namespace algorithm

using algorithm::equal;

} // namespace srook

#endif
