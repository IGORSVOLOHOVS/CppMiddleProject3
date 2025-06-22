#include <gtest/gtest.h>
#include <unordered_set> 
#include "heterogeneous_lookup.hpp"
#include "book.hpp"

using namespace bookdb;

TEST(TestTransparentString, True) { 
    bookdb::Book b1; b1.author = "Orwell"; b1.title = "1984"; b1.year = 1949; b1.genre = bookdb::Genre::SciFi; b1.rating = 4.8; b1.read_count = 200;
    bookdb::Book b2; b2.author = "Tolkien"; b2.title = "The Hobbit"; b2.year = 1937; b2.genre = bookdb::Genre::Fiction; b2.rating = 4.9; b2.read_count = 300;

    std::unordered_set<Book, TransparentStringHash, TransparentStringEqual> books{
        b1, b2
    };

    auto it = books.find("Orwell");

    ASSERT_NE(it, books.end());
    EXPECT_EQ(it->title, "1984");
}
