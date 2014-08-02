#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE flatten_tests
#include <boost/test/unit_test.hpp>

#include <list>
#include <vector>
#include <map>

#include "flatten/flatten.hpp"

typedef std::vector<int> IntList;
typedef std::vector<IntList> IntListList;
typedef std::vector<IntListList> IntListListList;

IntListListList example1_values()
{
    IntListListList values;
    IntListList a;
    IntList a1;
    IntList a2;
    IntList a3;
    IntListList b;
    IntList b1;
    IntList b2;
    IntList b3;

    a1.push_back(1);
    a1.push_back(2);
    a1.push_back(3);

    a2.push_back(4);
    a2.push_back(5);
    a2.push_back(6);

    /* a3 empty */

    b1.push_back(7);
    
    b2.push_back(8);
    
    b3.push_back(9);

    a.push_back(a1);
    a.push_back(a2);
    a.push_back(a3);

    b.push_back(b1);
    b.push_back(b2);
    b.push_back(b3);

    values.push_back(a);
    values.push_back(b);
    return values;
}

std::multimap<int, int> example2_values()
{
    std::multimap<int, int> result;
    result.insert(std::make_pair(1, 2));
    result.insert(std::make_pair(1, 3));
    result.insert(std::make_pair(2, 4));
    return result;
}

BOOST_AUTO_TEST_CASE(basic)
{
    IntListListList values = example1_values();
    IntList result;

    flatten::flatten<int>(
        values.begin(), values.end(), std::back_inserter(result)
    );

    for (int i = 0; i < 9; ++i)
    {
        BOOST_REQUIRE(i < result.size());
        BOOST_CHECK_EQUAL(result[i], i + 1);
    }
}

BOOST_AUTO_TEST_CASE(map)
{
    std::multimap<int, int> values = example2_values();
    IntList result;
    flatten::flatten<int>(
        values.begin(), values.end(), std::back_inserter(result)
    );

    BOOST_REQUIRE(result.size() == 3);
    BOOST_CHECK_EQUAL(result[0], 2);
    BOOST_CHECK_EQUAL(result[1], 3);
    BOOST_CHECK_EQUAL(result[2], 4);
}
