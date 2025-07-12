#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const Book& a, const Book & b){
        return a.author<b.author;
    }
};

struct GreaterByAuthor{
    bool operator()(const Book& a, const Book & b){
        return a.author>b.author;
    }
};

struct LessByYear{
    bool operator()(const Book& a, const Book & b){
        return a.year<b.year;
    }
};

struct GreaterByYear{
    bool operator()(const Book& a, const Book & b){
        return a.year>b.year;
    }
};

struct GreaterByRating{
    bool operator()(const Book& a, const Book & b){
        return a.rating>b.rating;
    }
};

struct LessByRating{
    bool operator()(const Book& a, const Book & b){
        return a.rating<b.rating;
    }
};

}  // namespace bookdb::comp