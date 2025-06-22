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
    if(cont.empty()){
        return {};
    }

    std::flat_map<std::string_view, int, Comparator> h;
    std::for_each(cont.begin(), cont.end(),[&h](const auto& book){
        h[book.author]++;
    });
    return h;
}

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto calculateGenreRatings(const BookDatabase<T> &cont, Comparator comp = {}) {
    if(cont.empty()){
        return {};
    }

    std::flat_map<Genre, double, Comparator> h;
    std::flat_map<Genre, int, Comparator> count;
    std::for_each(cont.begin(), cont.end(),[&h, &count](const auto& book){
        h[book.genre] += book.genre;
        count[book.genre]++;
    });
    std::for_each(cont.begin(), cont.end(),[&h, &count](const auto& book){
        h[book.genre] /= count[book.genre];
    });

    return h;
}

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto calculateAverageRating(const BookDatabase<T> &cont, Comparator comp = {}) {
    if(cont.empty()){
        return {};
    }

    auto total = std::reduce(cont.begin(), cont.end(), 0, [](const auto& lhs, const auto& rhs){
        return lhs.rating + rhs.rating;
    });

    return total / cont.size();
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t count) {
}

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto getTopNBy(const BookDatabase<T> &cont, Comparator comp = {}) {}

}  // namespace bookdb
