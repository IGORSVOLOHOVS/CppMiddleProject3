#pragma once

#include <format>
#include <stdexcept>
#include <string_view>
#include <flat_map>

using namespace std::string_view_literals;

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь
constexpr Genre GenreFromString(std::string_view s) {
    if (!s.compare("Fiction"sv)) return Genre::Fiction;
    else if(!s.compare("Mystery"sv)) return Genre::Mystery;
    else if(!s.compare("NonFiction"sv)) return Genre::NonFiction;
    else if(!s.compare("SciFi"sv)) return Genre::SciFi;
    else if(!s.compare("Biography"sv)) return Genre::Biography;
    else if(!s.compare("Unknown"sv)) return Genre::Unknown;

    throw std::logic_error{"Unsupported bookdb::Genre"};
}

constexpr std::string StringFromGenre(Genre g) {
    std::string genre_str;

    using bookdb::Genre;
    switch (g) {
        case Genre::Fiction:    genre_str = "Fiction"; break;
        case Genre::Mystery:    genre_str = "Mystery"; break;
        case Genre::NonFiction: genre_str = "NonFiction"; break;
        case Genre::SciFi:      genre_str = "SciFi"; break;
        case Genre::Biography:  genre_str = "Biography"; break;
        case Genre::Unknown:    genre_str = "Unknown"; break;
        default:
            throw std::logic_error{"Unsupported bookdb::Genre"};
    }
    return genre_str;
}


struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    Book() = default;
    constexpr Book(std::string_view s): genre(GenreFromString(s)){}
    constexpr Book(Genre g): genre(g){}

    auto operator<=>(const Book& other) const = default;
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        return format_to(fc.out(), "{}", StringFromGenre(g));
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

// Ваш код для std::formatter<Book> здесь
template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book& b, FormatContext &fc) const {
        return format_to(fc.out(), "Author: {} Title: {} Year: {} Genre: {} Rating: {} Read count: {}",
            b.author,
            b.title,
            b.year,
            StringFromGenre(b.genre),
            b.rating,
            b.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
