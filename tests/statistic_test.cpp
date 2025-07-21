#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "statsistics.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

using namespace bookdb;

class StatisticsTest : public ::testing::Test {
public:
    BookDatabase<> db;
    BookDatabase<> emptyDb;

    void SetUp() override {
        db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
        db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
        db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
        db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    }
};

TEST_F(StatisticsTest, AuthorHistogram) {
    auto histogram = buildAuthorHistogramFlat(db);
    EXPECT_EQ(histogram.size(), 4);
    EXPECT_EQ(histogram["George Orwell"], 2);
    EXPECT_EQ(histogram["F. Scott Fitzgerald"], 1);
    auto emptyHistogram = buildAuthorHistogramFlat(emptyDb);
    EXPECT_TRUE(emptyHistogram.empty());
}

TEST_F(StatisticsTest, GenreRatings) {
    auto ratings = calculateGenreRatings(db.cbegin(), db.cend());
    EXPECT_EQ(ratings.size(), 2);
    EXPECT_NEAR(ratings[Genre::SciFi].rating, 4.0, 1e-9);
    EXPECT_NEAR(ratings[Genre::Fiction].rating, 4.6, 1e-9);
    auto emptyRatings = calculateGenreRatings(emptyDb.cbegin(), emptyDb.cend());
    EXPECT_TRUE(emptyRatings.empty());
}

TEST_F(StatisticsTest, AverageRating) {
    double avg = calculateAverageRating(db);
    EXPECT_NEAR(avg, 4.48, 1e-9);
    EXPECT_EQ(calculateAverageRating(emptyDb), 0.0);
}

TEST_F(StatisticsTest, GetTopNByRating) {
    auto top = getTopNBy(db, 3, comp::GreaterByRating{});
    EXPECT_EQ(top.size(), 3);
    EXPECT_EQ(top[0].get().title, "To Kill a Mockingbird");
    EXPECT_EQ(top[1].get().title, "Pride and Prejudice");
    EXPECT_EQ(top[2].get().title, "The Great Gatsby");
}

TEST_F(StatisticsTest, GetTopNByPopularityNegative) {
    ASSERT_THROW(getTopNBy(db, 10, bookdb::comp::GreaterBypopularity{}), std::logic_error);
}
