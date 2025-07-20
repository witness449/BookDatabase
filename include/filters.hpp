#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
auto YearBetween(int lowYear, int highYear) {
    return [lowYear, highYear](const Book &b) { return b.year <= highYear && b.year >= lowYear; };
};

auto RatingAbove(double lowRating) {
    return [lowRating](const Book &b) { return b.rating > lowRating; };
};

auto GenreIs(bookdb::Genre genre) {
    return [genre](const Book &b) { return b.genre == genre; };
};

template <BookPredicate... Predicates>
auto all_of(Predicates... predicates) {
    return [=](const Book &b) { return (predicates(b) && ...); };
};

template <BookPredicate... Predicates>
auto any_of(Predicates... predicates) {
    return [=](const Book &b) { return (predicates(b) || ...); };
};

template <BookIterator it, BookPredicate p>
std::vector<std::reference_wrapper<bookdb::Book>> filterBooks(it first, it last, p pred) {
    std::vector<std::reference_wrapper<bookdb::Book>> result;
    auto distance=std::distance(first, last);
    result.reserve(distance);
    std::copy_if(first, last, std::back_inserter(result), pred);
    return result;
}

}  // namespace bookdb