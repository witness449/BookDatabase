#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr Genre GenreFromString(std::string_view s) {
    if(s=="Fiction"){
        return Genre::Fiction;
    }
    else if (s=="NonFiction"){
        return Genre::NonFiction;
    }
    else if(s=="SciFi"){
        return Genre::SciFi;
    }
    else if(s=="Biography"){
        return Genre::Biography;
    }
    else if(s=="Mystery"){
        return Genre::Mystery;
    }
    return Genre::Unknown;
}

constexpr std::string_view StringFromGenre(Genre genre){
    switch(genre){
        case Genre::Fiction: return "Fiction";
        case Genre::NonFiction: return "NonFiction";
        case Genre::SciFi: return "SciFi";
        case Genre::Biography: return "Biography";
        case Genre::Mystery: return "Mystery";
        default: return "Unknown";
    }
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    // Ваш код для конструкторов здесь
    constexpr Book(int year, Genre genre, double rating, int read_count):year(year),genre(genre),rating(rating),read_count(read_count){};
    constexpr Book(int year, std::string_view genre, double rating, int read_count):year(year),genre(GenreFromString(genre)),rating(rating),read_count(read_count){};
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}// namespace std

// Ваш код для std::formatter<Book> здесь

namespace std {
    template <>
    struct formatter<bookdb::Book, char> {
        template <typename FormatContext>
        auto format(const bookdb::Book b, FormatContext &fc) const {
            std::string genre_str;
    
            // clang-format off
            using bookdb::Genre;
            switch (b.genre) {
                case Genre::Fiction:    genre_str = "Fiction"; break;
                case Genre::Mystery:    genre_str = "Mystery"; break;
                case Genre::NonFiction: genre_str = "NonFiction"; break;
                case Genre::SciFi:      genre_str = "SciFi"; break;
                case Genre::Biography:  genre_str = "Biography"; break;
                case Genre::Unknown:    genre_str = "Unknown"; break;
                default:
                    throw logic_error{"Unsupported bookdb::Book"};
                }
            // clang-format on
            return format_to(fc.out(), "{}", genre_str);
        }
    
        constexpr auto parse(format_parse_context &ctx) {
            return ctx.begin();  // Просто игнорируем пользовательский формат
        }
    };

}  // namespace std
