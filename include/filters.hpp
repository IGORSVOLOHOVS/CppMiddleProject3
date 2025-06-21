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

    template <typename... Predicates>
    auto all_of(Predicates... preds) {
        return [=](const Book& book) { 
            return (preds(book) && ...);
        };
    }

    template <typename... Predicates>
    auto any_of(Predicates... preds) {
        return [=](const Book& book) { 
            return (preds(book) || ...);
        };
    }


    template <typename InputIt, typename Predicate>
    std::vector<std::reference_wrapper<const bookdb::Book>> filterBooks(
        InputIt first, InputIt last, Predicate p) {
        
        std::vector<std::reference_wrapper<const bookdb::Book>> result;
        result.reserve(std::distance(first, last)); 

        std::for_each(first, last, [&](const auto& element) {
            if (p(element)) {
                result.emplace_back(std::cref(element)); 
            }
        });
        
        return result;
    }
}  // namespace bookdb