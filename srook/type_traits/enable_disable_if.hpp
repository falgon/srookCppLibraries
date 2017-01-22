#ifdef __cplusplus
#ifndef INCLUDED_SROOK_TYPE_TRAITS_ENABLE_IF
#define INCLUDED_SROOK_TYPE_TRAITS_ENABLE_IF
#include<cstddef>
#if __has_include(<type_traits>)
#include<type_traits>
#define POSSIBLE_TO_INCLUDE_STD_ENABLE_IF
#elif __has_include(<boost/utility/enable_if.hpp>) && __has_include(<boost/utility/disable_if.hpp>)
#define POSSIBLE_TO_INCLUDE_BOOST_ENABLE_IF
#include<boost/utility/enable_if.hpp>
#include<boost/utility/disable_if.hpp>
#else
namespace srook{
inline namespace v1{

#ifdef SROOK_NO_CXX11_NULLPTR
template<bool B,class T=void>
#else
template<bool B,class T=std::nullptr_t>
#endif
struct enable_if{
	typedef T type;
};
template<class T>
struct enable_if<false,T>{};


#ifdef SROOK_NO_CXX11_NULLPTR
template<bool B,class T=void>
#else
template<bool B,class T=std::nullptr_t>
#endif
struct disable_if{};
template<class T>
struct disable_if<false,T>{
	typedef T type;
};

#ifndef SROOK_NO_CXX11_TEMPLATE_ALIASES
template<bool B,class T=std::nullptr_t>
using enable_if_t=typename enable_if<B,T>::type;

template<bool B,class T=std::nullptr_t>
using disable_if_t=typename disable_if<B,T>::type;
#endif

} // inline namespace v1
} // namespace srook
#endif
#endif
#endif
