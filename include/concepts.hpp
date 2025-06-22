#pragma once

#include <concepts>
#include <iterator>
#include <type_traits>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T c, const T cc, typename T::value_type v) {
    requires std::same_as<typename T::value_type, Book>;
    requires std::regular<T>;
    requires std::swappable<T>;
    requires std::destructible<typename T::value_type>;
    requires std::same_as<typename T::reference, typename T::value_type&>;
    requires std::same_as<typename T::const_reference, const typename T::value_type&>;
    requires std::forward_iterator<typename T::iterator>;
    requires std::forward_iterator<typename T::const_iterator>;
    
    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
    { cc.cbegin() } -> std::same_as<typename T::const_iterator>;
    { cc.cend() } -> std::same_as<typename T::const_iterator>;
    { c.size() } -> std::same_as<typename T::size_type>;
    { c.emplace_back(v) };
    { c.clear() };
};


template <typename T>
concept BookIterator = std::random_access_iterator<T>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = std::predicate<P, const Book&>;

template <typename C>
concept BookComparator = std::relation<C, const Book&, const Book&>;

}  // namespace bookdb