#pragma once

#include <format>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

using std::operator""sv;

constexpr Genre GenreFromString(std::string_view s) {
    if (!s.compare("Fiction"sv)) {
        return Genre::Fiction;
    } else if (!s.compare("NonFiction"sv)) {
        return Genre::NonFiction;
    } else if (!s.compare("SciFi"sv)) {
        return Genre::SciFi;
    } else if (!s.compare("Biography"sv)) {
        return Genre::Biography;
    } else if (!s.compare("Mystery"sv)) {
        return Genre::Mystery;
    }
    return Genre::Unknown;
}

constexpr std::string_view StringFromGenre(Genre genre) {
    switch (genre) {
    case Genre::Fiction:
        return "Fiction";
    case Genre::NonFiction:
        return "NonFiction";
    case Genre::SciFi:
        return "SciFi";
    case Genre::Biography:
        return "Biography";
    case Genre::Mystery:
        return "Mystery";
    default:
        return "Unknown";
    }
}

struct Book {
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    constexpr Book(std::string title, std::string_view author, int year, Genre genre, double rating, int read_count)
        : author(author), title(title), year(year), genre(genre), rating(rating), read_count(read_count) {};
    constexpr Book(std::string title, std::string_view author, int year, std::string_view genre, double rating,
                   int read_count)
        : author(author), title(title), year(year), genre(GenreFromString(genre)), rating(rating),
          read_count(read_count) {};
    Book() = default;

    auto operator<=>(const Book &other) const = default;

    friend auto operator<=>(const Book &l, std::string_view r) { return l.author <=> r; }
    friend auto operator<=>(const std::string &l, const Book &r) { return l <=> r.author; }
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        return format_to(fc.out(), "{}", bookdb::StringFromGenre(g));
    }
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book b, FormatContext &fc) const {
        return format_to(fc.out(), "Author: {}, Title: {}, Year: {}, Genre: {}, Rating: {}, Read count: {}", b.author,
                         b.title, b.year, bookdb::StringFromGenre(b.genre), b.rating, b.read_count);
    }
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

}  // namespace std
