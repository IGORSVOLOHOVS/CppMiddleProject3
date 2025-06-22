#include <gtest/gtest.h>
#include <vector>
#include <functional>
#include <list>

#include "concepts.hpp"
#include "book.hpp"
#include "comparators.hpp"

using namespace bookdb;


TEST(ConceptsTest, StaticAssertions) {
    static_assert(BookContainerLike<std::vector<Book>>);

    static_assert(BookIterator<std::vector<Book>::iterator>);
    
    static_assert(BookSentinel<std::vector<Book>::iterator, std::vector<Book>::iterator>);
    static_assert(BookSentinel<std::vector<Book>::const_iterator, std::vector<Book>::iterator>);
    
    static_assert(BookPredicate<std::function<bool(const Book&)>>);
    static_assert(!BookPredicate<int>);
    
    static_assert(BookComparator<comp::LessByRating>);
    static_assert(!BookComparator<int>);
}
