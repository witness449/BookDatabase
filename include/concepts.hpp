#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = true;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

template <typename T>
concept BookIterator = std::input_iterator<T> &&
    std::output_iterator<T, typename std::iterator_traits<T>::value_type>;

template <typename P>
concept BookPredicate = requires (P p) {
    { p(Book(1990, "Unknown", 1.1, 1))}; 
};

template <typename C>
concept BookComparator = requires (C c) {
    { c(Book(1990, "Unknown", 1.1, 1), Book(1990, "Unknown", 1.1, 1)) }; 
};
}  // namespace bookdb