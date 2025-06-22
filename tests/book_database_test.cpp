#include <gtest/gtest.h>
#include <print>
#include "book_database.hpp"
#include "comparators.hpp"

using namespace bookdb;

class BookDatabaseTest : public ::testing::Test {
protected:
    BookDatabase<std::vector<Book>> db;
    void SetUp() override {
        db.EmplaceBack("George Orwell", "1984", 1949, Genre::SciFi, 4.8, 200);
        db.EmplaceBack("George Orwell", "Animal Farm", 1945, Genre::Fiction, 4.4, 150);
        db.EmplaceBack("J.R.R. Tolkien", "The Hobbit", 1937, Genre::Fiction, 4.9, 300);
    }
};

TEST_F(BookDatabaseTest, InitializerListConstructor) {
    BookDatabase db_init{
        {"Author", "Title", 2000, Genre::Fiction, 4.0, 50},
        {"George Orwell", "1984", 1949, Genre::SciFi, 4.8, 200}
    };
    EXPECT_EQ(db_init.size(), 2);
    EXPECT_EQ(db_init.GetAuthors().size(), 2);
    EXPECT_EQ(db_init[0].title, "Title");
    EXPECT_EQ(db_init[1].author, "George Orwell");
}

TEST_F(BookDatabaseTest, EmplaceAndPushBack) {
    EXPECT_EQ(db.size(), 3);
    EXPECT_EQ(db.GetAuthors().size(), 2); 
    EXPECT_TRUE(db.GetAuthors().contains("George Orwell"));
}

TEST_F(BookDatabaseTest, Clear) {
    db.Clear();
    EXPECT_TRUE(db.empty());
    EXPECT_TRUE(db.GetAuthors().empty());
}

TEST_F(BookDatabaseTest, Accessors) {
    EXPECT_EQ(db[0].title, "1984");
    EXPECT_EQ(db.at(1).title, "Animal Farm");
    EXPECT_THROW(db.at(10), std::out_of_range);
}

TEST_F(BookDatabaseTest, Sorting) {
    auto& books = db.GetBooks();
    std::sort(books.begin(), books.end(), comp::LessByYear{});
    EXPECT_EQ(books[0].title, "The Hobbit");
    EXPECT_EQ(books[1].title, "Animal Farm");
    EXPECT_EQ(books[2].title, "1984");
}
