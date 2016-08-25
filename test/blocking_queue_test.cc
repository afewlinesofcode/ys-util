#define BOOST_TEST_MODULE Blocking Queue Tests
#include <boost/test/included/unit_test.hpp>

#include <ys/blocking_queue.h>

#include <cstdlib>
#include <queue>

BOOST_AUTO_TEST_SUITE(blocking_queue_test_suite)

BOOST_AUTO_TEST_CASE(empty_test)
{
    ys::blocking_queue<int> q {};

    BOOST_TEST(q.empty());

    q.push(1);

    BOOST_TEST(!q.empty());
}

BOOST_AUTO_TEST_CASE(size_test)
{
    ys::blocking_queue<int> q {};

    std::size_t s = std::rand() % 100 + 100;

    for (std::size_t i = 0; i < s; ++i)
        q.push(i);

    BOOST_TEST(q.size() == s);
}

BOOST_AUTO_TEST_CASE(values_test)
{
    ys::blocking_queue<int> q {};
    std::queue<int> q1 {};
    int v;

    std::size_t s = std::rand() % 100 + 100;

    for (std::size_t i = 0; i < s; ++i)
    {
        v = std::rand();
        q.push(v);
        q1.push(v);
    }

    while (!q.empty() && !q1.empty())
    {
        BOOST_TEST(q.pop(&v));
        BOOST_TEST(v == q1.front());
        q1.pop();
    }
}

BOOST_AUTO_TEST_SUITE_END()

