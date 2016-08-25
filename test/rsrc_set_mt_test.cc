#define BOOST_TEST_MODULE Resources Set Tests
#include <boost/test/included/unit_test.hpp>

#include <ys/rsrc_set_mt.h>
#include <set>

#include <cstdlib>

BOOST_AUTO_TEST_SUITE(rsrc_set_mt_test_suite)

BOOST_AUTO_TEST_CASE(size_test)
{
    ys::rsrc_set_mt<int> set {};

    std::size_t size = std::rand() % 100 + 100;

    for (std::size_t i = 0; i < size; ++i)
        set.put(std::make_shared<int>(i));

    BOOST_TEST(set.size() == size);
}

BOOST_AUTO_TEST_CASE(values_test)
{
}

BOOST_AUTO_TEST_SUITE_END()

