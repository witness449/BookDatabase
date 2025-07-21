#include "book.hpp"
#include "book_database.hpp"
#include <deque>
#include <gtest/gtest.h>

using namespace bookdb;

class BookDatabaseTest : public ::testing::Test {
public:
    BookDatabase<std::deque<Book>> db;
    void SetUp() override {}
};

TEST_F(BookDatabaseTest, BookConstructor) {
    Book b{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    db.PushBack(b);
    EXPECT_EQ(db.size(), 1);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    EXPECT_EQ(db.size(), 2);
    EXPECT_EQ(db.GetAuthors().size(), 1);
    EXPECT_EQ(db.GetAuthors().contains("George Orwell"), true);
}

TEST_F(BookDatabaseTest, ConstIterator) {
    auto it = db.cbegin();
    for (const Book &val : db) {
        ASSERT_NE(it, db.cend());
        EXPECT_EQ(*it, val);
        ++it;
    }
    EXPECT_EQ(it, db.cend());
}

TEST_F(BookDatabaseTest, EmptyDatabase) {
    EXPECT_TRUE(db.empty());
    EXPECT_TRUE(db.GetAuthors().empty());
    EXPECT_TRUE(db.begin() == db.end());
}