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
typedef std::multimap<int, int> IntMap;
typedef std::multimap<int, IntMap> IntMapMap;

namespace boost 
{

namespace test_tools
{

template<> 
struct print_log_value<IntList>
{
    void operator()(std::ostream& os, const IntList& l)
    {
        std::copy(l.begin(), l.end(), std::ostream_iterator<int>(os, " "));
    }
};

}

}

template<class C>
IntList simple_flatten(const C& c)
{
    IntList result;
    flatten::flatten<int>(
        c.begin(), c.end(), std::back_inserter(result));
    return result;
}

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

IntMap example2_values()
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
    IntMap values = example2_values();
    IntList result;
    flatten::flatten<int>(
        values.begin(), values.end(), std::back_inserter(result)
    );

    BOOST_REQUIRE_EQUAL(result.size(), 3);
    BOOST_CHECK_EQUAL(result[0], 2);
    BOOST_CHECK_EQUAL(result[1], 3);
    BOOST_CHECK_EQUAL(result[2], 4);
}

BOOST_AUTO_TEST_CASE(nested_map)
{
    IntMapMap values;
    IntMap v2;
    v2.insert(std::make_pair(0, 7));
    v2.insert(std::make_pair(0, 8));
    v2.insert(std::make_pair(0, 9));
    values.insert(std::make_pair(2, example2_values()));
    values.insert(std::make_pair(1, v2));
    values.insert(std::make_pair(0, IntMap()));
   
    IntList result = simple_flatten(values);

    BOOST_REQUIRE_EQUAL(result.size(), 6);
    BOOST_CHECK_EQUAL(result[0], 7);
    BOOST_CHECK_EQUAL(result[1], 8);
    BOOST_CHECK_EQUAL(result[2], 9);
    BOOST_CHECK_EQUAL(result[3], 2);
    BOOST_CHECK_EQUAL(result[4], 3);
    BOOST_CHECK_EQUAL(result[5], 4);
}

/* An iterator that asserts when you dereference it. */
struct fatal_iterator : 
    public std::iterator<
        std::output_iterator_tag,
        void, void, void, void
    >
{
    fatal_iterator operator*() const
    {
        BOOST_ERROR("Dereferencing fatal_iterator");
    }

    fatal_iterator operator++() { return fatal_iterator(); }
    fatal_iterator operator++(int) { return fatal_iterator(); }
    template<class T>
    fatal_iterator& operator=(const T&) { return *this; }
};

BOOST_AUTO_TEST_CASE(nested_empty)
{
    IntListListList il;
    il.push_back(IntListList());

    IntListList n;
    n.push_back(IntList());
    n.push_back(IntList());
    il.push_back(n);

    /* Verify we don't write anything to the output. */
    flatten::flatten<int>(il.begin(), il.end(), fatal_iterator());
}

BOOST_AUTO_TEST_CASE(preserve_empty_seq)
{
    std::map<int, IntListList> values;
    IntListList v0;
    v0.push_back(IntList());
    values.insert(make_pair(0, v0));

    IntListList result;
    flatten::flatten<IntList>(
        values.begin(), values.end(), std::back_inserter(result)
    );

    BOOST_REQUIRE_EQUAL(result.size(), 1);
    BOOST_CHECK_EQUAL(result[0], IntList());
}
