#pragma once

#include "book.hpp"
#include <concepts>
#include <cstddef>
#include <iterator>

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T b, const T cb) {
    { b.begin() } -> std::same_as<typename T::iterator>;
    { b.end() } -> std::same_as<typename T::iterator>;
    { b.size() } -> std::same_as<size_t>;
    { cb.cbegin() } -> std::same_as<typename T::const_iterator>;
    { cb.cend() } -> std::same_as<typename T::const_iterator>;
    { b.empty() } -> std::same_as<bool>;
    requires std::swappable<T>;
    requires std::same_as<typename T::value_type, Book>;
};

template <typename T, typename U>
concept BookSentinel = requires(T a, U b) {
    { a == b.end() } -> std::same_as<bool>;
};

template <typename T>
concept BookIterator = std::random_access_iterator<T>;

template <typename P>
concept BookPredicate = requires(P p) {
    { p(Book()) };
};

template <typename C>
concept BookComparator = requires(C c) {
    { c(Book(), Book()) } -> std::same_as<bool>;
};

}  // namespace bookdb
