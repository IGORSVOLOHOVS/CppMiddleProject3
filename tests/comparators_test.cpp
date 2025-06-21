#include <gtest/gtest.h>

#include "comparators.hpp"

using namespace bookdb;

TEST(TestLessComparators, True) { 
    bookdb::Book b1; b1.author = "1984"; b1.title = "George Orwell2"; b1.year = 1944; b1.genre = bookdb::Genre::SciFi; b1.rating = 1.; b1.read_count = 194;
    bookdb::Book b2; b2.author = "1983"; b2.title = "George Orwell1"; b2.year = 1943; b2.genre = bookdb::Genre::Fiction; b2.rating = 2.; b2.read_count = 190;
    bookdb::Book b3; b3.author = "1985"; b3.title = "George Orwell4"; b3.year = 1945; b3.genre = bookdb::Genre::Mystery; b3.rating = 4.1; b3.read_count = 190;
    bookdb::Book b4; b4.author = "1982"; b4.title = "George Orwe";  b4.year = 1942; b4.genre = bookdb::Genre::NonFiction; b4.rating = 0.1; b4.read_count = 191;
    bookdb::Book b5; b5.author = "1981"; b5.title = "George Orwellll"; b5.year = 1941; b5.genre = bookdb::Genre::Biography; b5.rating = 2.2; b5.read_count = 192;

    {
        std::vector<Book> books = {
            b1, b2, b3, b4, b5
        };

        std::sort(books.begin(), books.end(), comp::LessByAuthor{});

        EXPECT_EQ(books.at(0), b5);
        EXPECT_EQ(books.at(1), b4);
        EXPECT_EQ(books.at(2), b2);
        EXPECT_EQ(books.at(3), b1);
        EXPECT_EQ(books.at(4), b3);
    }


    {
        std::vector<Book> books = {
            b1, b2, b3, b4, b5
        };

        std::sort(books.begin(), books.end(), comp::LessByTitle{});

        EXPECT_EQ(books.at(0), b4);
        EXPECT_EQ(books.at(1), b2);
        EXPECT_EQ(books.at(2), b1);
        EXPECT_EQ(books.at(3), b3);
        EXPECT_EQ(books.at(4), b5);
    }


    {
        std::vector<Book> books = {
            b1, b2, b3, b4, b5
        };

        std::sort(books.begin(), books.end(), comp::LessByRating{});

        EXPECT_EQ(books.at(0), b4);
        EXPECT_EQ(books.at(1), b1);
        EXPECT_EQ(books.at(2), b2);
        EXPECT_EQ(books.at(3), b5);
        EXPECT_EQ(books.at(4), b3);
    }

    {
        std::vector<Book> books = {
            b1, b2, b3, b4, b5
        };

        std::sort(books.begin(), books.end(), comp::LessByGenre{});

        EXPECT_EQ(books.at(0), b2);
        EXPECT_EQ(books.at(1), b4);
        EXPECT_EQ(books.at(2), b1);
        EXPECT_EQ(books.at(3), b5);
        EXPECT_EQ(books.at(4), b3);
    }

    {
        std::vector<Book> books = {
            b1, b2, b3, b4, b5
        };

        std::sort(books.begin(), books.end(), comp::LessByYear{});

        EXPECT_EQ(books.at(0), b5);
        EXPECT_EQ(books.at(1), b4);
        EXPECT_EQ(books.at(2), b2);
        EXPECT_EQ(books.at(3), b1);
        EXPECT_EQ(books.at(4), b3);
    }

    {
        std::vector<Book> books = {
            b1, b2, b3, b4, b5
        };

        std::sort(books.begin(), books.end(), comp::LessByPopularity{});

        EXPECT_EQ(books.at(0), b2);
        EXPECT_EQ(books.at(1), b3);
        EXPECT_EQ(books.at(2), b4);
        EXPECT_EQ(books.at(3), b5);
        EXPECT_EQ(books.at(4), b1);
    }
}