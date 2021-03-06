// Copyright (C) 2011-2020 Roki. Distributed under the MIT License
#ifndef INCLUDED_SROOK_THREAD_SAFE_CONTAINER_PRIORITY_QUEUE_HPP
#define INCLUDED_SROOK_THREAD_SAFE_CONTAINER_PRIORITY_QUEUE_HPP

#include <srook/config/cpp_predefined.hpp>
#if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT
#    include <algorithm>
#    include <functional>
#    include <iterator>
#    include <srook/condition_variable.hpp>
#    include <srook/config.hpp>
#    include <srook/thread/safe/container/config.hpp>
#    include <srook/scope/unique_resource.hpp>
#    include <srook/memory/uses_allocator.hpp>
#    include <srook/mutex/mutexes/mutex.hpp>
#    include <srook/mutex/guards.hpp>
#    include <srook/type_traits.hpp>
#    include <vector>

namespace srook {
namespace threading {
namespace safe {
SROOK_INLINE_NAMESPACE(v1)

template <
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<SROOK_DEDUCED_TYPENAME Container::value_type>,
    behavior = behavior::wait_pushed>
class priority_queue;

namespace detail {

template <class T, class Container, class Compare>
class priority_queue_impl {
public:
    typedef SROOK_DEDUCED_TYPENAME Container::value_type value_type;
    typedef SROOK_DEDUCED_TYPENAME Container::reference reference;
    typedef SROOK_DEDUCED_TYPENAME Container::const_reference const_reference;
    typedef SROOK_DEDUCED_TYPENAME Container::size_type size_type;
    typedef Container container_type;
    typedef Compare value_compare;

#    if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT
#        define USES_ALLOC(X) SROOK_REQUIRES(uses_allocator<container_type, X>::value)
    template <class C = Container, SROOK_REQUIRES(is_default_constructible<C>::value)>
    priority_queue_impl() : c(), comp() {}

    explicit priority_queue_impl(const value_compare& com, const container_type& con)
        : c(con), comp(com)
    {
        std::make_heap(c.begin(), c.end(), comp);
    }
    explicit priority_queue_impl(value_compare&& com, container_type&& con = container_type())
        : c(srook::move(con)), comp(srook::move(com))
    {
        std::make_heap(c.begin(), c.end(), comp);
    }

    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue_impl(const Alloc& a) : c(a), comp() {}

    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue_impl(const value_compare& con, const Alloc& a) : c(a), comp(con) {}

    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue_impl(const value_compare& com, const container_type& con, const Alloc& a) : c(con, a), comp(com) {}

    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue_impl(const value_compare& com, container_type&& con, const Alloc& a) : c(srook::move(con), a), comp(com) {}

    template <class InputIterator>
    priority_queue_impl(InputIterator first, InputIterator last, const value_compare& com, const container_type& con)
        : c(con), comp(com)
    {
        c.insert(c.end(), first, last);
        std::make_heap(c.begin(), c.end(), comp);
    }
    template <class InputIterator>
    priority_queue_impl(InputIterator first, InputIterator last, const value_compare& com = value_compare(), container_type&& con = container_type())
        : c(srook::move(con)), comp(com)
    {
        c.insert(c.end(), first, last);
        std::make_heap(c.begin(), c.end(), comp);
    }
#    else
    explicit priority_queue_impl(const value_compare& com = value_compare(), const container_type& con = container_type())
        : c(con), comp(com) {}
    template <class InputIterator>
    priority_queue_impl(InputIterator first, InputIterator last, const value_compare& com = value_compare(), const container_type& con = container_type())
        : c(con), comp(com)
    {
        c.insert(c.end(), first, last);
        std::make_heap(c.begin(), c.end(), comp);
    }
#    endif
protected:
    container_type c;
    value_compare comp;
    mutex m_;

private:
    struct push_heaper_ {
        push_heaper_(const value_compare& vc) : vc_(vc) {}
        void operator()(container_type& c) const { std::push_heap(c.begin(), c.end(), vc_); }
    private:
        const value_compare& vc_;
    };
    typedef unique_resource<container_type&, push_heaper_> scoped_push_heap;

public:
    SROOK_FORCE_INLINE bool empty()
    SROOK_MEMFN_NOEXCEPT(declval<container_type>().empty())
    {
        srook::lock_guard<mutex> lk(m_);
        return c.empty();
    }

    SROOK_FORCE_INLINE size_type size()
    SROOK_MEMFN_NOEXCEPT(declval<container_type>().size())
    {
        srook::lock_guard<mutex> lk(m_);
        return c.size();
    }

    void push(const value_type& x)
    {
        srook::lock_guard<mutex> lk(m_);
        scoped_push_heap sh(c, push_heaper_(comp));
        c.push_back(x);
    }

#    if SROOK_CPP_RVALUE_REFERENCES
    void push(value_type&& x)
    {
        srook::lock_guard<mutex> lk(m_);
        scoped_push_heap sh(c, push_heaper_(comp));
        c.push_back(srook::move(x));
    }

    template <class... Args>
    auto emplace(Args&&... args)
    -> decltype(declval<container_type>().emplace_back(declval<Args>()...))
    {
        srook::lock_guard<mutex> lk(m_);
        scoped_push_heap sh(c, push_heaper_(comp));
        return c.emplace_back(srook::forward<Args>(args)...);
    }
#    endif

    void swap(priority_queue_impl& other)
    {
        if (this == srook::addressof(other)) return;
        using std::swap;

        srook::lock(m_, other.m_);
        srook::scoped_lock<mutex, mutex> lk(adopt_lock, m_, other.m_);
        swap(c, other.c);
    }
};

} // namespace detail

template <class T, class Container, class Compare, behavior>
class priority_queue : public detail::priority_queue_impl<T, Container, Compare> {
    typedef detail::priority_queue_impl<T, Container, Compare> base_type;
    struct check_notifier_ : public enable_copy_move<true, true, true, true, check_notifier_> {
        check_notifier_(const priority_queue& st) : st_(st) {}
        SROOK_FORCE_INLINE void operator()(condition_variable& cv) const
        {
            if (!st_.c.empty()) cv.notify_all();
        }

    private:
        const priority_queue& st_;
    };
    struct notifier_ {
        SROOK_FORCE_INLINE void operator()(condition_variable& cv) const { cv.notify_all(); }
    };
    typedef unique_resource<condition_variable&, check_notifier_> scoped_check_notifier;
    typedef unique_resource<condition_variable&, notifier_> scoped_notifier;

public:
    typedef SROOK_DEDUCED_TYPENAME base_type::value_type value_type;
    typedef SROOK_DEDUCED_TYPENAME base_type::reference reference;
    typedef SROOK_DEDUCED_TYPENAME base_type::const_reference const_reference;
    typedef SROOK_DEDUCED_TYPENAME base_type::size_type size_type;
    typedef SROOK_DEDUCED_TYPENAME base_type::container_type container_type;
    typedef SROOK_DEDUCED_TYPENAME base_type::value_compare value_compare;

protected:
    srook::condition_variable cv_empty_check;
    bool is_aborted_
#    if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT
        = false
#    endif
        ;
public:
#    if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT && SROOK_CPP_RVALUE_REFERENCES
    template <class C = container_type, SROOK_REQUIRES(is_default_constructible<C>::value)>
    priority_queue()
        : base_type() {}
    explicit priority_queue(const value_compare& com, const container_type& con)
        : base_type(com, con) {}
    explicit priority_queue(const value_compare& com, container_type&& con = container_type())
        : base_type(com, srook::move(con)) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(const Alloc& a)
        : base_type(a) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(const value_compare& com, const Alloc& a)
        : base_type(com, a) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(const value_compare& com, const container_type& con, const Alloc& a)
        : base_type(com, con, a) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(const value_compare& com, container_type&& con, const Alloc& a)
        : base_type(com, srook::move(con), a) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(const priority_queue& s, const Alloc& a)
        : base_type(s.c, a, s.comp) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(priority_queue&& s, const Alloc& a)
        : base_type(srook::move(s.c), a, s.comp) {}
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last, const value_compare& com, const container_type& con)
        : base_type(first, last, com, con) {}
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last, const value_compare& com = value_compare(), container_type&& con = container_type())
        : base_type(first, last, com, srook::move(con)) {}
#    else
    explicit priority_queue(const value_compare& com = value_compare(), const container_type& con = container_type())
        : base_type(com, con), is_aborted_(false) {}
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last, const value_compare& com = value_compare(), const container_type& con = container_type())
        : base_type(first, last, com, con) {}
#    endif

    void push(const value_type& x)
    {
        scoped_check_notifier n_(cv_empty_check, check_notifier_(*this));
        base_type::push(x);
    }

#    if SROOK_CPP_RVALUE_REFERENCES
    void push(value_type&& x)
    {
        scoped_check_notifier n_(cv_empty_check, check_notifier_(*this));
        base_type::push(srook::move(x));
    }

    template <class... Args>
    auto emplace(Args&&... args)
    -> decltype(declval<container_type>().emplace_back(declval<Args>()...))
    {
        scoped_check_notifier n_(cv_empty_check, check_notifier_(*this));
        return base_type::emplace(srook::forward<Args>(args)...);
    }
#    endif

    void abort()
    {
        scoped_notifier n_(cv_empty_check, notifier_());
        srook::lock_guard<mutex> lk(this->m_);
        is_aborted_ = true;
    }

    const_reference top()
    {
        srook::unique_lock<mutex> lk(this->m_);
        cv_empty_check.wait(lk, [this] { return !this->c.empty() || is_aborted_; });
        if (is_aborted_) throw aborted();
        return this->c.front();
    }

    srook::libraries::optional<value_type> pop()
    {
        srook::unique_lock<mutex> lk(this->m_);
        cv_empty_check.wait(lk, [this] { return !this->c.empty() || is_aborted_; });
        if (is_aborted_) throw aborted();

        const value_type result(this->c.front());
        std::pop_heap(this->c.begin(), this->c.end(), this->comp);
        this->c.pop_back();
        return srook::libraries::make_optional(result);
    }

    void swap(priority_queue& other)
    SROOK_MEMFN_NOEXCEPT(is_nothrow_swappable<container_type>::value)
    {
        if (this == srook::addressof(other)) return;
        using std::swap;

        srook::lock(this->m_, other.m_);
        srook::scoped_lock<mutex> lk(adopt_lock, this->m_, other.m_);
        swap(this->c, other.c);
        swap(is_aborted_, other.is_aborted_);
    }
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator==(const priority_queue<U, C>&, const priority_queue<U, C>&);
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator<(const priority_queue<U, C>&, const priority_queue<U, C>&);
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator!=(const priority_queue<U, C>&, const priority_queue<U, C>&);
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator>(const priority_queue<U, C>&, const priority_queue<U, C>&);
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator<=(const priority_queue<U, C>&, const priority_queue<U, C>&);
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator>=(const priority_queue<U, C>&, const priority_queue<U, C>&);
};

template <class T, class Container, class Compare>
struct priority_queue<T, Container, Compare, behavior::no_wait_pushed> : public detail::priority_queue_impl<T, Container, Compare> {
    typedef detail::priority_queue_impl<T, Container, Compare> base_type;

public:
    typedef SROOK_DEDUCED_TYPENAME base_type::value_type value_type;
    typedef SROOK_DEDUCED_TYPENAME base_type::reference reference;
    typedef SROOK_DEDUCED_TYPENAME base_type::const_reference const_reference;
    typedef SROOK_DEDUCED_TYPENAME base_type::size_type size_type;
    typedef SROOK_DEDUCED_TYPENAME base_type::container_type container_type;
    typedef SROOK_DEDUCED_TYPENAME base_type::value_compare value_compare;

public:
#    if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT && SROOK_CPP_RVALUE_REFERENCES
    template <class C = container_type, SROOK_REQUIRES(is_default_constructible<C>::value)>
    priority_queue()
        : base_type() {}
    explicit priority_queue(const value_compare& com, const container_type& con)
        : base_type(com, con) {}
    explicit priority_queue(const value_compare& com, container_type&& con = container_type())
        : base_type(com, srook::move(con)) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(const Alloc& a)
        : base_type(a) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(const value_compare& com, const Alloc& a)
        : base_type(com, a) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(const value_compare& com, const container_type& con, const Alloc& a)
        : base_type(com, con, a) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(const value_compare& com, container_type&& con, const Alloc& a)
        : base_type(com, srook::move(con), a) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(const priority_queue& s, const Alloc& a)
        : base_type(s.c, a, s.comp) {}
    template <class Alloc, USES_ALLOC(Alloc)>
    explicit priority_queue(priority_queue&& s, const Alloc& a)
        : base_type(srook::move(s.c), a, s.comp) {}
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last, const value_compare& com, const container_type& con)
        : base_type(first, last, com, con) {}
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last, const value_compare& com = value_compare(), container_type&& con = container_type())
        : base_type(first, last, com, srook::move(con)) {}
#    else
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last, const value_compare& com = value_compare(), const container_type& con = container_type())
        : base_type(first, last, com, con) {}
    explicit priority_queue(const container_type& c = container_type())
        : base_type(c) {}
#        undef USES_ALLOC
#    endif

#    ifdef __glibcxx_check_nonempty
#        define CHECK_NONEMPTY() __glibcxx_check_nonempty()
#    else
#        define CHECK_NONEMPTY() assert(this->c.size())
#    endif
    const_reference top() const
    {
        srook::lock_guard<mutex> lk(this->m_);
        CHECK_NONEMPTY();
        return this->c.front();
    }
    srook::libraries::optional<value_type> pop()
    {
        if (this->empty()) return srook::libraries::nullopt;

        srook::lock_guard<mutex> lk(this->m_);
        CHECK_NONEMPTY();
        const value_type result(this->c.front());
        this->c.pop_front();
        return srook::libraries::make_optional(result);
    }
#    undef CHECK_NONEMPTY
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator==(const priority_queue<U, C>&, const priority_queue<U, C>&);
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator<(const priority_queue<U, C>&, const priority_queue<U, C>&);
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator!=(const priority_queue<U, C>&, const priority_queue<U, C>&);
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator>(const priority_queue<U, C>&, const priority_queue<U, C>&);
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator<=(const priority_queue<U, C>&, const priority_queue<U, C>&);
    template <class U, class C>
    friend SROOK_FORCE_INLINE bool operator>=(const priority_queue<U, C>&, const priority_queue<U, C>&);
};

template <class T, class Container, class Compare>
SROOK_FORCE_INLINE bool operator==(const priority_queue<T, Container, Compare>& lhs, const priority_queue<T, Container, Compare>& rhs)
{
    srook::lock(lhs.m_, rhs.m_);
    srook::scoped_lock<mutex, mutex> lk(adopt_lock, lhs.m_, rhs.m_);
    return lhs.c == rhs.c;
}

template <class T, class Container, class Compare>
SROOK_FORCE_INLINE bool operator<(const priority_queue<T, Container, Compare>& lhs, const priority_queue<T, Container, Compare>& rhs)
{
    srook::lock(lhs.m_, rhs.m_);
    srook::scoped_lock<mutex, mutex> lk(adopt_lock, lhs.m_, rhs.m_);
    return lhs.c < rhs.c;
}

template <class T, class Container, class Compare>
SROOK_FORCE_INLINE bool operator!=(const priority_queue<T, Container, Compare>& lhs, const priority_queue<T, Container, Compare>& rhs)
{
    srook::lock(lhs.m_, rhs.m_);
    srook::scoped_lock<mutex, mutex> lk(adopt_lock, lhs.m_, rhs.m_);
    return !(lhs == rhs);
}

template <class T, class Container, class Compare>
SROOK_FORCE_INLINE bool operator>(const priority_queue<T, Container, Compare>& lhs, const priority_queue<T, Container, Compare>& rhs)
{
    srook::lock(lhs.m_, rhs.m_);
    srook::scoped_lock<mutex, mutex> lk(adopt_lock, lhs.m_, rhs.m_);
    return rhs < lhs;
}

template <class T, class Container, class Compare>
SROOK_FORCE_INLINE bool operator<=(const priority_queue<T, Container, Compare>& lhs, const priority_queue<T, Container, Compare>& rhs)
{
    srook::lock(lhs.m_, rhs.m_);
    srook::scoped_lock<mutex, mutex> lk(adopt_lock, lhs.m_, rhs.m_);
    return !(rhs < lhs);
}

template <class T, class Container, class Compare>
SROOK_FORCE_INLINE bool operator>=(const priority_queue<T, Container, Compare>& lhs, const priority_queue<T, Container, Compare>& rhs)
{
    srook::lock(lhs.m_, rhs.m_);
    srook::scoped_lock<mutex, mutex> lk(adopt_lock, lhs.m_, rhs.m_);
    return !(lhs < rhs);
}

template <class T, class Container, class Compare>
inline SROOK_DEDUCED_TYPENAME enable_if<type_traits::detail::Land<is_swappable<Container>, is_swappable<Compare> >::value>::type
swap(priority_queue<T, Container, Compare>& lhs, priority_queue<T, Container, Compare>& rhs)
SROOK_NOEXCEPT(lhs.swap(rhs))
{
    lhs.swap(rhs);
}

#    if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS17_CONSTANT
template <class Compare, class Container>
priority_queue(Compare, Container)
-> priority_queue<SROOK_DEDUCED_TYPENAME Container::value_type, Container, Compare, behavior::wait_pushed>;

template <
    class InputIterator,
    class Compare = std::less<SROOK_DEDUCED_TYPENAME std::iterator_traits<InputIterator>::value_type>,
    class Container = std::vector<SROOK_DEDUCED_TYPENAME std::iterator_traits<InputIterator>::value_type> >
priority_queue(InputIterator, InputIterator, Compare = Compare(), Container = Container())
-> priority_queue<SROOK_DEDUCED_TYPENAME std::iterator_traits<InputIterator>::value_type, Container, Compare, behavior::wait_pushed>;

template <class Compare, class Container, class Allocator>
priority_queue(Compare, Container, Allocator)
-> priority_queue<SROOK_DEDUCED_TYPENAME Container::value_type, Container, Compare, behavior::wait_pushed>;
#    endif

SROOK_INLINE_NAMESPACE_END
} // namespace safe
} // namespace threading

namespace thread_safe {
namespace container {

using threading::safe::priority_queue;
using threading::safe::swap;

} // namespace container
} // namespace thread_safe

template <class T, class Container, class Compare, class Alloc>
struct uses_allocator<thread_safe::container::priority_queue<T, Container, Compare>, Alloc>
    : public uses_allocator<Container, Alloc>::type {};

} // namespace srook

#    if SROOK_CPLUSPLUS >= SROOK_CPLUSPLUS11_CONSTANT
#        include <type_traits>

namespace std {

template <class T, class Container, class Compare, class Alloc>
struct uses_allocator<srook::thread_safe::container::priority_queue<T, Container, Compare>, Alloc>
    : public uses_allocator<Container, Alloc>::type {};

} // namespace std

#        endif
#    endif
#endif
