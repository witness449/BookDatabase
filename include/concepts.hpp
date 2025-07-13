#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T b) {
    b.begin();
    b.end();
};

// template <typename T, typename Container, typename = typename SameType<T, typename Container::value_type>::type>
// class Stack { /*тело класса*/
// };;

/*template <typename Container>
requires IterableContainer<Container>
void print_container(const Container& container) {
    for (const auto& element : container) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}*/

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

template <typename T>
concept BookIterator = std::random_access_iterator<T>;
// std::input_iterator<T> &&
//     std::output_iterator<T, typename std::iterator_traits<T>::value_type>;

template <typename P>
concept BookPredicate = requires(P p) {
    { p(Book()) };
};

template <typename C>
concept BookComparator = requires(C c) {
    { c(Book(), Book()) };
};
}  // namespace bookdb
