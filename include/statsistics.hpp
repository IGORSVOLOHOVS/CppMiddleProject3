#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"

#include <print>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    const auto& authors = cont.GetAuthors();
    const auto& books = cont.GetBooks();

    std::flat_map<std::string_view, int, Comparator> h;
    std::for_each(books.begin(), books.end(),[&h](const auto& book){
        h[book.author]++;
    });
    return h;
}

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto calculateGenreRatings(const BookDatabase<T> &cont, Comparator comp = {}) {

}

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto calculateAverageRating(const BookDatabase<T> &cont, Comparator comp = {}) {}

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto sampleRandomBooks(const BookDatabase<T> &cont, Comparator comp = {}) {}

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto getTopNBy(const BookDatabase<T> &cont, Comparator comp = {}) {}

}  // namespace bookdb
