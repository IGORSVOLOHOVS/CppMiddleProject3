#include <gtest/gtest.h>
#include "filters.hpp"
#include "book_database.hpp"

using namespace bookdb;

class FiltersTest : public ::testing::Test {
protected:
    BookDatabase<> db;
    void SetUp() override {
        db.EmplaceBack("George Orwell", "1984", 1949, Genre::SciFi, 4.8, 200);
        db.EmplaceBack("George Orwell", "Animal Farm", 1945, Genre::Fiction, 4.4, 150);
        db.EmplaceBack("J.R.R. Tolkien", "The Hobbit", 1937, Genre::Fiction, 4.9, 300);
        db.EmplaceBack("Aldous Huxley", "Brave New World", 1932, Genre::SciFi, 4.5, 180);
    }
};

TEST_F(FiltersTest, FilterByYear) {
    auto books_1940s = filterBooks(db.begin(), db.end(), YearBetween(1940, 1949));
    EXPECT_EQ(books_1940s.size(), 2);
    EXPECT_EQ(books_1940s[0].get().title, "1984");
    EXPECT_EQ(books_1940s[1].get().title, "Animal Farm");
}

TEST_F(FiltersTest, FilterByGenre) {
    auto scifi_books = filterBooks(db.begin(), db.end(), GenreIs(Genre::SciFi));
    EXPECT_EQ(scifi_books.size(), 2);
}

TEST_F(FiltersTest, FilterByRating) {
    auto high_rating_books = filterBooks(db.begin(), db.end(), RatingAbove(4.6));
    EXPECT_EQ(high_rating_books.size(), 2); 
}

TEST_F(FiltersTest, AllOfComposition) {
    auto predicate = all_of(GenreIs(Genre::SciFi), RatingAbove(4.6));
    auto result = filterBooks(db.begin(), db.end(), predicate);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].get().title, "1984");
}

TEST_F(FiltersTest, AnyOfComposition) {
    auto predicate = any_of(YearBetween(1930, 1940), RatingAbove(4.85));
    auto result = filterBooks(db.begin(), db.end(), predicate);
    EXPECT_EQ(result.size(), 2);
}

TEST_F(FiltersTest, EmptyResult) {
    auto result = filterBooks(db.begin(), db.end(), GenreIs(Genre::Mystery));
    EXPECT_TRUE(result.empty());
}
