#pragma once

#include <algorithm>
#include <flat_map>
#include <flat_set>
#include <iterator>
#include <numeric>
#include <random>
#include <type_traits>
#include <utility>

#include "book.hpp"
#include "book_database.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

struct RatingCount {
    double rating = 0;
    int count = 0;
    bool modified = false;
};

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &b, TransparentStringLess Comp = {}) {
    // Контейнер с книгами на основе прозрачного компаратора
    std::flat_multiset<Book, Comparator> bookSet(Comp);
    // Контейнер с авторами
    std::flat_map<std::string, int> resultMap;

    if (b.empty()) {
        return resultMap;
    }

    for_each(b.cbegin(), b.cend(), [&](const Book &book) { bookSet.insert(book); });

    auto authors = b.GetAuthors();
    for (const auto &a : authors) {
        resultMap.emplace(a, bookSet.count(a));
    }
    return resultMap;
}

template <BookIterator it>
auto calculateGenreRatings(it i1, it i2) {
    std::flat_map<bookdb::Genre, bookdb::RatingCount> flatRatingCount;

    if (i1 == i2) {
        return flatRatingCount;
    }

    // Ассерт на случай неконстантного итератора
    static_assert(std::is_const_v<std::remove_reference_t<decltype(*i1)>>, "Iterator must point to a const type.");
    static_assert(std::is_const_v<std::remove_reference_t<decltype(*i2)>>, "Iterator must point to a const type.");

    std::for_each(i1, i2, [&](const auto &b) {
        auto iter = flatRatingCount.lower_bound(b.genre);
        if (iter != flatRatingCount.end() && iter->first == b.genre) {
            iter->second.count++;
            iter->second.rating += b.rating;
        } else {
            flatRatingCount.emplace(std::make_pair(b.genre, RatingCount{b.rating, 1, false})); 
        }
    });

    auto ratings{std::move(flatRatingCount).extract()};
    for (auto &x : ratings.values) {
        x.rating = x.rating / x.count;
        x.modified = true;
    }

    flatRatingCount.replace(std::move(ratings.keys), std::move(ratings.values));

    return flatRatingCount;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &books) {
    if (books.empty()) {
        return 0.0;
    }
    double r =
        std::accumulate(books.cbegin(), books.cend(), 0.0, [](double sum, const Book &b) { return sum += b.rating; });
    return r / books.size();
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &books, size_t n) {
    std::vector<Book> sampleBooks;

    if (n == 0 || books.empty()) {
        return sampleBooks;
    }

    sampleBooks.reserve(n);
    const auto &m_books = books.GetBooks();
    std::sample(m_books.cbegin(), m_books.cend(), std::back_inserter(sampleBooks), n,
                std::mt19937{std::random_device{}()});

    return sampleBooks;
}

template <typename T, BookComparator Comp>
auto getTopNBy(BookDatabase<T> &books, size_t n, Comp comp) {
    std::vector<std::reference_wrapper<const Book>> topBooks;
    if (n == 0) {
        return topBooks;
    }
    if (n > books.size()) {
        throw std::logic_error("N greater than database size");
    }
    auto &m_books = books.GetBooks();
    std::partial_sort(m_books.begin(), m_books.begin() + n, m_books.end(), comp);

    topBooks.reserve(n);
    std::transform(m_books.begin(), m_books.begin() + n, std::back_inserter(topBooks),
                   [](const Book &book) { return std::cref(book); });
    return topBooks;
}

}  // namespace bookdb

namespace std {
template <>
struct formatter<std::flat_map<std::string, int>> {
    template <typename FormatContext>
    auto format(const std::flat_map<std::string, int> &m, FormatContext &fc) const {
        auto out = fc.out();
        for (const auto &x : m) {

            out = format_to(out, "\nAuthor {} ", x.first);
            for (int i = 0; i < x.second; i++) {
                out = format_to(out, "*");
            }
        }

        return out;
    }
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

template <>
struct formatter<std::flat_map<bookdb::Genre, bookdb::RatingCount>> {
    template <typename FormatContext>
    auto format(std::flat_map<bookdb::Genre, bookdb::RatingCount> &m, FormatContext &fc) const {
        auto out = fc.out();
        for (const auto &x : m) {
            out = format_to(out, "\nGenre {}, Rating {} ", bookdb::StringFromGenre(x.first), x.second.rating);
        }
        return out;
    }
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

}  // namespace std
