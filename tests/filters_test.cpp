#include "book.hpp"
#include "book_database.hpp"
#include "filters.hpp"
#include "heterogeneous_lookup.hpp"
#include <gtest/gtest.h>

using namespace bookdb;

class FiltersTest : public ::testing::Test {
protected:
    BookDatabase<> db;
    void SetUp() override {
        db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
        db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
        db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
        db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    }
};

TEST_F(FiltersTest, FilterByYear) {
    auto books20cent = filterBooks(db.begin(), db.end(), YearBetween(1900, 1999));
    EXPECT_EQ(books20cent.size(), 4);
}

TEST_F(FiltersTest, FilterByGenre) {
    auto scifi = filterBooks(db.begin(), db.end(), GenreIs(Genre::SciFi));
    EXPECT_EQ(scifi.size(), 1);
}

TEST_F(FiltersTest, FilterByRating) {
    auto highRating = filterBooks(db.begin(), db.end(), RatingAbove(4.7));
    EXPECT_EQ(highRating.size(), 1);
}

// Тестирование переменного числа предикатов и гетерогенного сравнения
TEST_F(FiltersTest, AllOfComposition) {
    auto allOf = all_of(YearBetween(1800, 1900), RatingAbove(4.6));
    auto result = filterBooks(db.begin(), db.end(), allOf);
    EXPECT_EQ(result.size(), 1);
    std::set<Book, TransparentStringLess> setResult(result.begin(), result.end());
    auto it = setResult.find("Jane Austen");
    EXPECT_EQ(it->year > 1800 && it->year < 1900 && it->rating > 4.6, true);
}

TEST_F(FiltersTest, AnyOfComposition) {
    auto predicate = any_of(GenreIs(Genre::SciFi), YearBetween(1950, 2025));
    auto result = filterBooks(db.begin(), db.end(), predicate);
    EXPECT_EQ(result.size(), 2);
    std::set<Book, TransparentStringLess> setResult(result.begin(), result.end());
    auto it = setResult.find("George Orwell");
    EXPECT_EQ(it->year > 1950 || it->year < 2025 || it->genre == Genre::SciFi, true);
}
