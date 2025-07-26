#pragma once

#include "book.hpp"
#include <concepts>
#include <cstddef>
#include <iterator>

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T b, const T cb, typename T::value_type book) {
    { b.begin() } -> std::same_as<typename T::iterator>;
    { b.end() } -> std::same_as<typename T::iterator>;
    { b.size() } -> std::same_as<size_t>;
    { cb.cbegin() } -> std::same_as<typename T::const_iterator>;
    { cb.cend() } -> std::same_as<typename T::const_iterator>;
    { b.empty() } -> std::same_as<bool>;
    { b.clear() };
    { b.push_back(book) };
    { b.emplace_back("1984", "George Orwell", 1949, Genre::SciFi, 4., 190) };

    requires std::swappable<T>;
    requires std::same_as<typename T::value_type, Book>;
};

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

template <typename T>
concept BookIterator = std::random_access_iterator<T>;

template <typename P>
concept BookPredicate = std::predicate<P, const Book &>;

template <typename C>
concept BookComparator = requires(C c, const Book &lhs, const Book &rhs) {
    { c(lhs, rhs) } -> std::convertible_to<bool>;
};

}  // namespace bookdb
