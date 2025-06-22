#include <gtest/gtest.h>

#include <print>

#include "book.hpp"

using namespace bookdb;

TEST(TestGenreFromString, True) { 
    EXPECT_EQ(GenreFromString("Fiction"sv), Genre::Fiction);
    EXPECT_EQ(GenreFromString("Mystery"sv), Genre::Mystery);
    EXPECT_EQ(GenreFromString("NonFiction"sv), Genre::NonFiction);
    EXPECT_EQ(GenreFromString("SciFi"sv), Genre::SciFi);
    EXPECT_EQ(GenreFromString("Biography"sv), Genre::Biography);
    EXPECT_EQ(GenreFromString("Unknown"sv), Genre::Unknown);

    EXPECT_THROW(GenreFromString("ErrorGenre"sv), std::logic_error);
}
TEST(TestStringFromGenre, True) { 
    EXPECT_EQ(StringFromGenre(Genre::Fiction), "Fiction"sv);
    EXPECT_EQ(StringFromGenre(Genre::Mystery), "Mystery"sv);
    EXPECT_EQ(StringFromGenre(Genre::NonFiction), "NonFiction"sv);
    EXPECT_EQ(StringFromGenre(Genre::SciFi), "SciFi"sv);
    EXPECT_EQ(StringFromGenre(Genre::Biography), "Biography"sv);
    EXPECT_EQ(StringFromGenre(Genre::Unknown), "Unknown"sv);
}

TEST(TestBookConstructors, True) {
    {
        Book book;
    }

    {
        constexpr auto expected_genre = Genre::SciFi;
        constexpr auto real_genre = GenreFromString("SciFi"sv);
        Book book(real_genre);

        EXPECT_EQ(book.genre, expected_genre);
    }

    {
        constexpr auto expected_genre = Genre::SciFi;
        Book book(expected_genre);

        EXPECT_EQ(book.genre, expected_genre);
    }
}

TEST(TestGenreFormat, True) { 
    const std::string expected_genre_str = "Genre: SciFi";
    const std::string real_genre_str = std::format("Genre: {}", Genre::SciFi);

    EXPECT_EQ(expected_genre_str, real_genre_str);
}

TEST(TestBookFormat, True) { 
    bookdb::Book b1; b1.author = "1984"; b1.title = "George Orwell2"; b1.year = 1944; b1.genre = bookdb::Genre::SciFi; b1.rating = 1.2; b1.read_count = 194;
    
    const std::string expected_book_str = "Author: 1984 Title: George Orwell2 Year: 1944 Genre: SciFi Rating: 1.2 Read count: 194";
    const std::string real_book_str = std::format("{}", b1);

    EXPECT_EQ(expected_book_str, real_book_str);
}
