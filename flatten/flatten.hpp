#ifndef FLATTEN_FLATTEN_HPP
#define FLATTEN_FLATTEN_HPP 

#include <algorithm>
#include <iterator>

namespace flatten
{

namespace detail
{

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
            typedef typename InType::const_iterator SubIt;
            typedef typename std::iterator_traits<SubIt>::value_type SubType;
            flatten_impl<SubType, OutType>::flatten(
                it->begin(), it->end(), out
            );
        }
    }
};

template<class T, class V, class OutType>
struct flatten_impl<std::pair<T, V>, OutType> 
{ 
    template<class InputIterator, class OutputIterator>
    static void flatten(const std::pair<T, V>& v, OutputIterator out,
        typename std::iterator_traits<InputIterator>::value_type::const_iterator* sfinae = 0)
    {
        typedef typename V::const_iterator SubIt;
        typedef typename std::iterator_traits<SubIt>::value_type SubType;
        flatten_impl<SubType, OutType>::flatten(
            v.second.begin(), v.second.end(), out
        );
    }

    template<class OutputIterator>
    static void flatten(const std::pair<T, V>& v, OutputIterator out)
    {
        *out++ = v.second;
    }

    template<class InputIterator, class OutputIterator>
    static void flatten(
        InputIterator begin, InputIterator end,
        OutputIterator out
    )
    {
        for (InputIterator it = begin; it != end; ++it)
        {
            flatten(*it, out);
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
