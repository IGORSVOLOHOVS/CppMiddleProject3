# Manual

## The library

```cpp
#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statistics.hpp"

using namespace bookdb;

BookDatabase<> db;
db.EmplaceBack("George Orwell", "1984", 1949, Genre::SciFi, 4.8, 200);
db.EmplaceBack("J.R.R. Tolkien", "The Hobbit", 1937, Genre::Fiction, 4.9, 300);

auto good = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
auto top = getTopNBy(db, 1, comp::GreaterByRating{});
auto histogram = buildAuthorHistogramFlat(db);
double average = calculateAverageRating(db);
```

`std::format` prints a `Book`, a `Genre`, a `BookDatabase<std::vector<Book>>` and a `bookdb::flat_map`. `filterBooks`, `sampleRandomBooks` and `getTopNBy` answer `std::reference_wrapper`s into the database, so they live only as long as it does. `getTopNBy` reorders the database it is given.

`GenreFromString` and `StringFromGenre` throw `std::logic_error` for a genre they do not know, and `sampleRandomBooks` and `getTopNBy` throw it for a count above the size of the database. `sampleRandomBooks` answers an empty list for a count of 0.

## The demo

`.\recipe.ps1 run` builds and runs `BookDB`, which fills a database with ten books and prints the sorts, the histogram, the ratings, a filter and a top three. It hands `EmplaceBack` the title first and the author second, as the course's example does, so the two columns read the wrong way round and the last lookup, by author, finds nothing.

## The tests

`.\recipe.ps1 build` leaves the test binary at `build\BookDB_tests.exe`, which takes any GoogleTest flag:

| command | what it does |
| --- | --- |
| `build\BookDB_tests.exe` | runs every case |
| `build\BookDB_tests.exe --gtest_filter=FiltersTest.*` | runs the filter cases |
| `build\BookDB_tests.exe --gtest_list_tests` | lists the cases |
