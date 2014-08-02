## cpp-flatten 
An header-only C++98 (or better) library that provides a function that
flattens nested sequences.

### Synopsis 
This library consists of one header file (`flatten.hpp`) which provides one
user-visible template function:

    template<class T, class InputIterator, class OutputIterator>
    void flatten(InputIterator begin, InputIterator end, OutputIterator out);

Given an iterator to an arbitrarily nested sequence, the function will produce
a flat sequence with values of type T. For example:

`````
typedef std::vector<int> IntList;
typedef std::vector<IntList> IntListList;
typedef std::vector<IntList> IntListListList;

/* Using C++11's initialization syntax for simplicity. */
IntListListList values 
{
    { { 1, 2, 3 }, { 4, 5, 6 }, { } },
    { { 7 }, { 8 }, { 9 } }
};

std::ostream_iterator<int> out(std::cout, " ");
flatten::flatten<int>(values.begin(), values.end(), out);
`````

When run, this snippet will print this sequence to std::out:

`````
1 2 3 4 5 6 7 8 9
`````

You can use a std::back\_inserter to convert to flattened list:

`````
template<class NestedList>
std::vector<int> flatten_vector(const NestedList& in)
{
    std::vector<int> result;
    flatten::flatten<int>(in.begin(), in.end(), std::back_inserter(result));
    return result;
} 
`````

`flatten` also supports flattening the values (though not the keys) in a map:

`````
std::multimap<int, int> values = { { 1, 2 }, { 1, 3 }, { 2, 4} };
std::ostream_iterator<int> out(std::cout, " ");
flatten::flatten<int>(values.begin(), values.end(), out);
`````

Produces:

`````
2 3 4
`````

Finally, `flatten` doesn't have to flatten a list entirely. Calling `flatten`
with `T = some list type` works too:

`````
IntListListList values = { /* */ };
IntListList flattened;
flatten::flatten<IntList>(
    values.begin(), 
    values.end(), 
    std::back_inserter(flattened)
);
`````

### Installation 
Flatten is a header-only library, so all you really need to do is copy 
flatten.hpp to wherever's convenient. However, if you'd like to install 
it into your system includes, you can install flatten with CMake. For
instance, on a Debian system:

`````
$ cmake <path-to-flatten-repo>
$ make
$ sudo make install
`````

The library also provides some unit tests, which require that CppUnit be
installed on your system. You can build and run the unit tests with CMake:

`````
$ cmake <path-to-flatten-repo>
$ make check
`````

If you run into issues, make sure CMake can find your CppUnit library
and headers. Otherwise...

### Reporting Bugs 
If you run into issues (bugs, compilation errors, unit test failures, etc.),
feel free to create an issue in GitHub's issue tracker. Please provide any
relevant details, like:

- your compiler, 
- your compiler version,
- your compilation flags, 
- the error messages or incorrect results you're seeing,
- your CMake version (if applicable),
- anything else you think is relevant.
