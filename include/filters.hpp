#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
    auto YearBetween(int start_year, int end_year) {
        return [start_year, end_year](const Book& book) {
            return book.year >= start_year && book.year <= end_year;
        };
    }

    auto RatingAbove(double min_rating) {
        return [min_rating](const Book& book) {
            return book.rating >= min_rating;
        };
    }

    auto GenreIs(Genre target_genre) {
        return [target_genre](const Book& book) {
            return book.genre == target_genre;
        };
    }

    template <BookPredicate... Predicates>
    auto all_of(Predicates... preds) {
        return [=](const Book& book) { 
            return (preds(book) && ...);
        };
    }

    template <BookPredicate... Predicates>
    auto any_of(Predicates... preds) {
        return [=](const Book& book) { 
            return (preds(book) || ...);
        };
    }


    template <std::input_iterator InputIt, BookPredicate Predicate>
    std::vector<std::reference_wrapper<const bookdb::Book>> filterBooks(InputIt first, InputIt last, Predicate p) {
        std::vector<std::reference_wrapper<const bookdb::Book>> result;
        if constexpr (std::forward_iterator<InputIt>) {
            auto dist = std::distance(first, last);
            if (dist > 0) {
                result.reserve(dist);
            }
        }

        std::copy_if(first, last, std::back_inserter(result), p);
        return result;
    }
}  // namespace bookdb