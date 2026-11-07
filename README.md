# cpp-middle-project3

A book database in modern C++: a container of `Book` values, concepts that say what a container, iterator, predicate and comparator must be, `std::format` support, composable filters and statistics. It began as the sprint 3 project of the Yandex Practicum "Middle C++ developer" course. The library is headers only and depends on nothing but the standard library.

## What it does

- **Books**: `Book`, `Genre`, and `GenreFromString` and `StringFromGenre`, which also work at compile time.
- **The database**: `BookDatabase<Container>`, a container of books that keeps each author's name once and hands out `std::string_view`s of it.
- **Comparators**: `comp::LessByAuthor`, `LessByTitle`, `LessByRating`, `LessByGenre`, `LessByYear`, `LessByPopularity`, and two `GreaterBy` ones.
- **Filters**: `YearBetween`, `RatingAbove` and `GenreIs`, joined by `all_of` and `any_of` and applied by `filterBooks`.
- **Statistics**: `buildAuthorHistogramFlat`, `calculateGenreRatings`, `calculateAverageRating`, `sampleRandomBooks` and `getTopNBy`.
- **Heterogeneous lookup**: `TransparentStringHash`, `TransparentStringEqual` and `TransparentStringLess`, which find a book by author with no `Book` built.

Four lines throw on purpose, and `.\recipe.ps1 test` pins them: both genre conversions throw `std::logic_error` for a genre they do not know, and `sampleRandomBooks` and `getTopNBy` throw it for a count above the size of the database.

## Layout

```text
include/  the library, as headers
src/      main.cpp, a console demo
tests/    GoogleTest cases
```

## Build and test

`.\recipe.ps1 test` builds the demo and the tests, runs `ctest`, `clang-tidy` and `clang-format`, and searches `include` and `src` for locks and for `throw`, `try` and `catch`. It needs mingw g++, cmake, ninja and LLVM. `docs/install.md` has the details, and `docs/manual.md` covers the library and the demo.

MIT licensed: `LICENSE` is the course's, copyright Yandex Practicum.

## Lab requirements

This repository complies with [revision 4.0.0](http://127.0.0.1:3000/igors-lab/software-requirements/src/tag/v4.0.0/BEST_REQUIREMENTS.md) of the lab requirements, except R-02 (#8), R-09 (#9), R-12 (#10), R-13 (#11), R-13 (#12), R-14 (#13), R-37 (#14), R-41 (#15), R-42 (#16).
