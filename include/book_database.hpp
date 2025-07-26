#pragma once

#include <initializer_list>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using value_type = typename BookContainer::value_type;
    using reference = typename BookContainer::reference;
    using const_reference = typename BookContainer::const_reference;
    using size_type = BookContainer::size_type;
    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;
    using diff_type = iterator::difference_type;

    using AuthorContainer = std::unordered_set<std::string>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> books) {
        books_.reserve(books.size());
        for (const auto &x : books) {
            auto it = authors_.insert(std::string{x.author.begin(), x.author.end()});
            books_.emplace_back(x.title, *it.first, x.year, x.genre, x.rating, x.read_count);
        }
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods
    void push(const value_type &value) {
        authors_.insert(std::string{value.author.begin(), value.author.end()});
        books_.push_back(value);
    }

    void push(value_type &&value) {
        authors_.insert(std::string{value.author.begin(), value.author.end()});
        books_.push_back(std::move(value));
    }

    void pop() { books_.pop_back(); }
    reference top() { return books_.back(); }
    const_reference top() const { return books_.back(); }
    bool empty() const { return books_.empty(); }
    size_type size() const { return books_.size(); }
    const_reference at(size_t n) const { return books_.at(n); }
    const_reference operator[](size_t n) const { return books_[n]; }
    reference at(size_t n) { return books_.at(n); }
    reference operator[](size_t n) { return books_[n]; }
    iterator begin() { return books_.begin(); }
    const_iterator cbegin() const { return books_.cbegin(); }
    iterator end() { return books_.end(); }
    const_iterator cend() const { return books_.cend(); }

    const BookContainer &GetBooks() const { return books_; }
    BookContainer &GetBooks() { return books_; }
    const AuthorContainer &GetAuthors() const { return authors_; }
    AuthorContainer &GetAuthors() { return authors_; }

    void EmplaceBack(std::string_view title, std::string_view author, int year, Genre genre, double rating,
                     int read_count) {
        auto it = authors_.emplace(std::string{author.begin(), author.end()});
        books_.emplace_back(title, *it.first, year, genre, rating, read_count);
    }

    void PushBack(const Book &b) {
        auto it = authors_.insert(std::string{b.author.begin(), b.author.end()});
        books_.push_back({b.title, *it.first, b.year, b.genre, b.rating, b.read_count});
    }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <bookdb::BookContainerLike BookContainer>
struct formatter<bookdb::BookDatabase<BookContainer>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<BookContainer> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());
        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }
        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};
}  // namespace std
