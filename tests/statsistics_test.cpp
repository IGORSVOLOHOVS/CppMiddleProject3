#include <gtest/gtest.h>

#include "statsistics.hpp"

using namespace bookdb;

TEST(TestStatistics, True) { 
    bookdb::Book b1; b1.author = "1984"; b1.title = "George Orwell2"; b1.year = 1944; b1.genre = bookdb::Genre::SciFi; b1.rating = 1.; b1.read_count = 194;
    bookdb::Book b2; b2.author = "1984"; b2.title = "George Orwell1"; b2.year = 1943; b2.genre = bookdb::Genre::Fiction; b2.rating = 2.; b2.read_count = 190;
    bookdb::Book b3; b3.author = "1985"; b3.title = "George Orwell4"; b3.year = 1945; b3.genre = bookdb::Genre::Mystery; b3.rating = 4.1; b3.read_count = 190;
    bookdb::Book b4; b4.author = "1982"; b4.title = "George Orwe";  b4.year = 1942; b4.genre = bookdb::Genre::NonFiction; b4.rating = 0.1; b4.read_count = 191;
    bookdb::Book b5; b5.author = "1982"; b5.title = "George Orwellll"; b5.year = 1941; b5.genre = bookdb::Genre::Biography; b5.rating = 2.2; b5.read_count = 192;

    {
        // buildAuthorHistogramFlat
    }
}