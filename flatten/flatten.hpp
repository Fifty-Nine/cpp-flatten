#ifndef FLATTEN_FLATTEN_HPP
#define FLATTEN_FLATTEN_HPP 

#include <algorithm>
#include <iterator>

namespace flatten
{

namespace detail
{

template<class T>
struct container_traits
{
    typedef typename T::const_iterator iterator;
    typedef typename std::iterator_traits<iterator>::value_type value_type;

    static iterator begin(const T& t) { return t.begin(); }
    static iterator end(const T& t) { return t.end(); }
};

template<class T, class S>
struct container_traits<std::pair<T, S> >
{
    typedef const S* iterator;
    typedef S value_type;

    static iterator begin(const std::pair<T, S>& v) { return &v.second; }
    static iterator end(const std::pair<T, S>& v) { return &v.second + 1; }
};

template<class InType, class OutType>
struct flatten_impl
{
    template<class InputIterator, class OutputIterator>
    static void flatten(
        InputIterator begin, InputIterator end, 
        OutputIterator out)
    {
        for (InputIterator it = begin; it != end; ++it)
        {
            typedef typename container_traits<InType>::iterator SubIt;
            typedef typename container_traits<InType>::value_type SubType;

            SubIt begin = container_traits<InType>::begin(*it);
            SubIt end = container_traits<InType>::end(*it);
            flatten_impl<SubType, OutType>::flatten(begin, end, out);
        }
    }
};

template<class T>
struct flatten_impl<T, T>
{
    template<class InputIterator, class OutputIterator>
    static void flatten(
        InputIterator begin, InputIterator end, 
        OutputIterator out)
    {
        std::copy(begin, end, out);
    }

};

}

template<class OutType, class InputIterator, class OutputIterator>
void flatten(
    InputIterator begin, InputIterator end, 
    OutputIterator out)
{
    typedef typename std::iterator_traits<InputIterator>::value_type InType;
    return detail::flatten_impl<InType, OutType>::flatten(begin, end, out);
}

} 

#endif /* FLATTEN_FLATTEN_HPP*/
