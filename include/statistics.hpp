#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"
#include "comparators.hpp" 
#include "heterogeneous_lookup.hpp" 

#include <print>

namespace bookdb {
    
template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string_view, int, Comparator> h(comp);
    if(cont.empty()){
        return h;
    }
    
    std::for_each(cont.cbegin(), cont.cend(), [&h](const Book& book) {
        const auto it = h.lower_bound(book.author);
        if (it != h.end() && it->first == book.author) {
            ++(it->second);
        } else {
            h.emplace_hint(it, book.author, 1);
        }
    });
    
    return h;
}

template <BookContainerLike T>
auto calculateGenreRatings(const BookDatabase<T>& db) {
    if (db.empty()) {
        return std::flat_map<Genre, double>{};
    }
    
    std::flat_map<Genre, std::pair<double, int>> ratings_and_counts;
    std::for_each(db.cbegin(), db.cend(), [&ratings_and_counts](const Book& book){
        auto it = ratings_and_counts.lower_bound(book.genre);
        if (it != ratings_and_counts.end() && it->first == book.genre) {
            it->second.first += book.rating;
            it->second.second++;
        } else {
            ratings_and_counts.emplace_hint(it, book.genre, std::pair{book.rating, 1});
        }
    });

    std::flat_map<Genre, double> avg_ratings;
    for (const auto& [genre, pair] : ratings_and_counts) {
        if (pair.second > 0) {
            avg_ratings.emplace(genre, pair.first / pair.second);
        }
    }
    return avg_ratings;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T>& db) {
    if (db.empty()) {
        return 0.0;
    }

    const double total_rating = std::accumulate(db.cbegin(), db.cend(), 0.0,
        [](double sum, const Book& book) {
            return sum + book.rating;
        });

    return total_rating / db.size();
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T>& db, size_t count) {
    std::vector<std::reference_wrapper<const Book>> sample_books;
    const auto& books = db.GetBooks();
    
    if (count == 0 || books.empty()) {
        return sample_books;
    }

    if(count > books.size()){
        throw std::logic_error("The count is more then books size");
    }
    
    std::sample(books.cbegin(), books.cend(), std::back_inserter(sample_books),
                count, std::mt19937{std::random_device{}()});

    return sample_books;
}

template <BookContainerLike T, BookComparator Comp>
auto getTopNBy(BookDatabase<T>& db, size_t n, Comp comp) {
    auto& books = db.GetBooks();

    if (n == 0) {
        return std::vector<std::reference_wrapper<const Book>>{};
    }

    if(n > books.size()){
        throw std::logic_error("The count is more then books size");
    }

    std::partial_sort(books.begin(), books.begin() + n, books.end(), comp);

    std::vector<std::reference_wrapper<const Book>> top_n_books;
    top_n_books.reserve(n);
    std::transform(books.begin(), books.begin() + n, std::back_inserter(top_n_books), 
                   [](const Book& book) { return std::cref(book); });

    return top_n_books;
}

}  // namespace bookdb
