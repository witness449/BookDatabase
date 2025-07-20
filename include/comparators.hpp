#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const Book &a, const Book &b) { return a.author < b.author; }
};

struct GreaterByAuthor {
    bool operator()(const Book &a, const Book &b) { return a.author > b.author; }
};

struct LessByYear {
    bool operator()(const Book &a, const Book &b) { return a.year < b.year; }
};

struct GreaterByYear {
    bool operator()(const Book &a, const Book &b) { return a.year > b.year; }
};

struct LessByRating {
    bool operator()(const Book &a, const Book &b) { return a.rating < b.rating; }
};

struct GreaterByRating {
    bool operator()(const Book &a, const Book &b) { return a.rating > b.rating; }
};

struct LessByPopularity {
    bool operator()(const Book &a, const Book &b) { return a.read_count < b.read_count; }
};

struct GreaterBypopularity {
    bool operator()(const Book &a, const Book &b) { return a.read_count > b.read_count; }
};

}  // namespace bookdb::comp