#pragma once

#include <concepts>
#include <iterator>
#include <type_traits>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T c) {
    typename T::value_type;
    typename T::iterator;
    typename T::const_iterator;
    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
    { c.cbegin() } -> std::same_as<typename T::const_iterator>;
    { c.cend() } -> std::same_as<typename T::const_iterator>;
    { c.size() } -> std::same_as<typename T::size_type>;
    { c.emplace_back(std::declval<typename T::value_type>()) };
    { c.push_back(std::declval<typename T::value_type>()) };
    { c.clear() };
};

template <typename T>
concept BookIterator = std::random_access_iterator<T>;

template <typename S, typename I>
concept BookSentinel = true;

template <typename P>
concept BookPredicate = true;

template <typename C>
concept BookComparator = true;

}  // namespace bookdb