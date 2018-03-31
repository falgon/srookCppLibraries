#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <srook/range/adaptor/transformed.hpp>
#include <srook/range/adaptor/copied.hpp>
#include <srook/range/adaptor/moved.hpp>
#include <srook/execution.hpp>
#include <srook/numeric/algorithm/iota.hpp>
#include <algorithm>
#include <vector>

BOOST_AUTO_TEST_SUITE(srook_range_adaptor_take_test)

BOOST_AUTO_TEST_CASE(intro_sort1)
{
    std::vector<int> v(1 << 5, 0);
    srook::numeric::algorithm::iota(srook::execution::par, v, 0);
    const auto l = [](SROOK_DEDUCED_TYPENAME SROOK_DECLTYPE(v)::value_type x) { return x * 2; };

    std::vector<int> res1 = v 
        | srook::range::adaptors::transformed(l) 
        | srook::range::adaptors::copied;

    std::vector<int> res2 = v
        | boost::adaptors::transformed(l)
        | srook::range::adaptors::moved;

    BOOST_TEST(boost::equal(res1, res2));
}

BOOST_AUTO_TEST_SUITE_END()
