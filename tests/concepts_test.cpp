#include <gtest/gtest.h>

#include "concepts.hpp"

using namespace bookdb;

TEST(TestConcepts, True) {
    static_assert(BookContainerLike<std::vector<Book>>);
    static_assert(BookIterator<std::vector<Book>::iterator>);
    static_assert(BookSentinel<int,int>);
    static_assert(BookPredicate<int>);
    static_assert(BookComparator<int>);
} 