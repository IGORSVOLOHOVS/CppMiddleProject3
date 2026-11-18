# cpp-middle-project3 - Software Requirements Specification

Reverse-engineered from commit `86d4eea` (tag `v1.0.0`) on 2026-11-18. Every requirement states only behaviour
present in the code at that commit. The documentation hub is [`docs/README.md`](docs/README.md).

## 1. Purpose and scope

The system is a header-only C++23 library, namespace `bookdb`, that stores books in memory and computes
selections and statistics over them, plus a console demo `BookDB` and a GoogleTest suite (`README.md`,
`CMakeLists.txt:23-27`).

In scope: the headers in `include/`, the demo `src/main.cpp`, the tests in `tests/*.cpp`, the CMake build.

Out of scope: persistence, file or network I/O, a user interface beyond console output, concurrency (none of
these exist in the code; `recipe.ps1:140` forbids lock words in `include` and `src`). The repository tooling
shared with repo-quality-template is covered only by FR-30.

## 2. Context

| Actor / system | Role | Source |
| --- | --- | --- |
| C++ developer | Includes the headers and calls the API | `docs/install.md`, `docs/manual.md` |
| Course | Origin: Yandex Practicum "Middle C++ developer", sprint 3 | `README.md`, `LICENSE` |
| GoogleTest 1.13.0 | Test framework, fetched at configure time | `CMakeLists.txt:33-43` |
| Gitea server and `windows` runner | Run `.gitea/workflows/*` | `.gitea/workflows/test.yaml:16` |
| Lab share (`LAB_SHARE`) | Receives `include` and `src` from `recipe.ps1 publish` | `recipe.ps1:107-117` |

Runtime environment: any C++23 compiler and standard library. The recipe targets Windows with mingw g++ 13+,
CMake 3.28+, Ninja and LLVM (`docs/install.md`, `recipe.ps1:51-62`). Code paths for compilers without `<print>`
or `<flat_map>` exist (`src/main.cpp:8-23`, `include/flat_map_compat.hpp:30-157`).

## 3. Functional requirements

### 3.1 Books and genres

- **FR-1** The system shall define `enum class Genre : std::uint8_t` with the values `Fiction`, `NonFiction`,
  `SciFi`, `Biography`, `Mystery`, `Unknown`. Source: `include/book.hpp:18`. Verification: `TestGenreFromString.True`.
- **FR-2** The system shall convert the six exact genre names to `Genre` in a `constexpr` function
  `GenreFromString` and throw `std::logic_error` for any other string. Source: `include/book.hpp:21-36`.
  Verification: `TestGenreFromString.True`, `TestBookConstructors.True`.
- **FR-3** The system shall convert each `Genre` value to its name with `StringFromGenre` and throw
  `std::logic_error` for a value outside the enumerators. Source: `include/book.hpp:38-65`. Verification:
  `TestStringFromGenre.True` (throw branch: no test).
- **FR-4** The system shall represent a book as `Book` with `author` (`std::string_view`), `title`
  (`std::string`), `year`, `genre`, `rating` (`double`), `read_count`, constructible from all six fields, from a
  genre name, or from a `Genre`, with a defaulted `operator<=>`. Source: `include/book.hpp:67-84`. Verification:
  `TestBookConstructors.True`, `TestLessComparators.True` (uses `==`); genre-name constructor: no test.
- **FR-5** The system shall format a `Genre` through `std::format` as its name. Its `parse` consumes nothing, so
  only an empty format spec is accepted: a non-empty one is a compile error in a literal format string and
  `std::format_error` in `std::vformat` (reproduced with clang++ 20 and the MSVC library). Source:
  `include/book.hpp:88-98`. Verification: `TestGenreFormat.True`.
- **FR-6** The system shall format a `Book` as `Author: {} Title: {} Year: {} Genre: {} Rating: {} Read count: {}`.
  Source: `include/book.hpp:101-112`. Verification: `TestBookFormat.True`.

### 3.2 Concepts

- **FR-7** The system shall define `BookContainerLike<T>`: `value_type` is `Book`, `T` is regular and
  swappable, has forward `iterator`/`const_iterator`, and provides `begin`, `end`, `cbegin`, `cend`, `size`,
  `emplace_back`, `clear`. Source: `include/concepts.hpp:12-30`. Verification: `ConceptsTest.StaticAssertions`.
- **FR-8** The system shall define `BookIterator` (random-access iterator), `BookSentinel` (`std::sentinel_for`),
  `BookPredicate` (predicate on `const Book &`) and `BookComparator` (relation on two `const Book &`).
  Source: `include/concepts.hpp:32-42`. Verification: `ConceptsTest.StaticAssertions`.

### 3.3 Book database

- **FR-9** The system shall provide `BookDatabase<Container = std::vector<Book>>`, constrained by
  `BookContainerLike`, that re-exports the container's member types and offers `begin`/`end`/`cbegin`/`cend`,
  `size`, `empty`, `reserve`, `capacity`. Source: `include/book_database.hpp:15-51`. Verification:
  `BookDatabaseTest.EmplaceAndPushBack`, `BookDatabaseTest.Clear`.
- **FR-10** The system shall store each author name once in an `std::unordered_set<std::string>` and set each
  stored book's `author` to a view of that set's string, in the initializer-list constructor, `EmplaceBack` and
  `PushBack`. Source: `include/book_database.hpp:28-61`. Verification: `BookDatabaseTest.InitializerListConstructor`,
  `BookDatabaseTest.EmplaceAndPushBack` (`PushBack`: no test).
- **FR-11** `Clear` shall empty both the books and the author set. Source: `include/book_database.hpp:63-66`.
  Verification: `BookDatabaseTest.Clear`.
- **FR-12** The system shall give unchecked `operator[]` and checked `at` (container's `std::out_of_range`).
  Source: `include/book_database.hpp:68-71`. Verification: `BookDatabaseTest.Accessors`.
- **FR-13** The system shall expose the containers through const and non-const `GetBooks` and `GetAuthors`.
  Source: `include/book_database.hpp:73-76`. Verification: `BookDatabaseTest.Sorting`.
- **FR-14** The system shall format a `BookDatabase<std::vector<Book>>` as `BookDatabase (size = N): Books:`,
  one `- <book>` line per book, `Authors:`, one `- <author>` line per author. Source:
  `include/book_database.hpp:86-107`. Verification: no test.

### 3.4 Comparators

- **FR-15** The system shall provide in `bookdb::comp` the functors `LessByAuthor`, `LessByTitle`,
  `LessByRating`, `LessByGenre`, `LessByYear`, `LessByPopularity` (`read_count`), `GreaterByRating`,
  `GreaterByPopularity`, each comparing one field. Source: `include/comparators.hpp:8-38`. Verification:
  `TestLessComparators.True`; Greater-by via `StatisticsTest.GetTopNByRating`, `StatisticsTest.GetTopNByPopularity`.

### 3.5 Filters

- **FR-16** The system shall build predicates `YearBetween(a, b)` (`a <= year <= b`), `RatingAbove(x)`
  (`rating >= x`) and `GenreIs(g)`. Source: `include/filters.hpp:11-21`. Verification: `FiltersTest.FilterByYear`,
  `FiltersTest.FilterByRating`, `FiltersTest.FilterByGenre`.
- **FR-17** The system shall combine `BookPredicate`s with `all_of` (fold `&&`) and `any_of` (fold `||`).
  Source: `include/filters.hpp:23-31`. Verification: `FiltersTest.AllOfComposition`, `FiltersTest.AnyOfComposition`.
- **FR-18** `filterBooks(first, last, pred)` shall return `std::vector<std::reference_wrapper<const Book>>` of
  the matching books in input order, reserving the range length for forward iterators. Source:
  `include/filters.hpp:33-45`. Verification: `FiltersTest.FilterByYear`, `FiltersTest.EmptyResult`.

### 3.6 Heterogeneous lookup

- **FR-19** The system shall provide `TransparentStringLess`, `TransparentStringEqual` and
  `TransparentStringHash` (`is_transparent`) that compare or hash a `Book` by `author` and accept
  `std::string_view` in place of a `Book`. Source: `include/heterogeneous_lookup.hpp:12-35`. Verification:
  `TestTransparentString.True` (Equal and Hash); Less via `StatisticsTest.AuthorHistogram`.

### 3.7 Statistics

- **FR-20** `buildAuthorHistogramFlat(db, comp = TransparentStringLess{})` shall return
  `flat_map<std::string_view, int, Comparator>` with the number of books per author, empty for an empty
  database. Source: `include/statistics.hpp:18-35`. Verification: `StatisticsTest.AuthorHistogram`.
- **FR-21** `calculateGenreRatings(first, last)` shall return `flat_map<Genre, double>` with the mean rating of
  each genre present, empty for an empty range. Source: `include/statistics.hpp:37-61`. Verification:
  `StatisticsTest.GenreRatings`.
- **FR-22** `calculateAverageRating(db)` shall return the mean rating, `0.0` for an empty database. Source:
  `include/statistics.hpp:63-73`. Verification: `StatisticsTest.AverageRating`.
- **FR-23** `sampleRandomBooks(db, count)` shall return an empty list when `count` is 0 or the database is
  empty, throw `std::logic_error` when `count` exceeds the size, and otherwise return `count` references chosen
  by `std::sample` with an `std::mt19937` seeded from `std::random_device`. Source:
  `include/statistics.hpp:75-92`. Verification: `StatisticsTest.SampleRandomBooks` (empty-database branch: no
  test).
- **FR-24** `getTopNBy(db, n, comp)` shall return an empty list for `n == 0`, throw `std::logic_error` for
  `n` above the size, and otherwise `partial_sort` the database's own container by `comp` and return references
  to its first `n` books. Source: `include/statistics.hpp:94-114`. Verification: `StatisticsTest.GetTopNByRating`,
  `StatisticsTest.GetTopNByPopularity` (`n == 0` and throw: no test).
- **FR-25** The system shall format a `bookdb::flat_map` as `{"key": value, ...}`. Source:
  `include/statistics.hpp:122-141`. Verification: no test.

### 3.8 Portability shims

- **FR-26** `bookdb::flat_map` shall be an alias of `std::flat_map` when `__cpp_lib_flat_map` is defined, and
  otherwise a sorted `std::vector<std::pair<Key, T>>` with `lower_bound`, `find`, `operator[]`, `emplace`,
  `emplace_hint` (hint ignored), iteration, `size`, `empty`, `clear`. Source: `include/flat_map_compat.hpp:30-157`.
  Verification: indirectly `StatisticsTest.AuthorHistogram`, `StatisticsTest.GenreRatings` (on whichever branch
  the compiler selects).
- **FR-27** The demo shall use `std::print` when `<print>` exists and otherwise a `print` built on `std::format`
  writing to `stdout`. Source: `src/main.cpp:8-23`. Verification: no test.

### 3.9 Demo

- **FR-28** `BookDB` shall fill a database with ten books (title passed as author, author as title) and print
  the database, the database sorted by author and by popularity, the author histogram, ratings per genre, the
  average rating, the books from 1900-1999 with rating at least 4.5, the first three by `comp::LessByRating`,
  and a `std::find_if` result for author `George Orwell` when one is found. Source: `src/main.cpp:39-87`.
  Verification: no test (run once by hand, see `docs/README.md`).

### 3.10 Build

- **FR-29** The build shall define the INTERFACE target `bookdb`, the executable `BookDB`, and with
  `BUILD_TESTING=ON` the executable `BookDB_tests` and the ctest entries `bookdb_tests` and `manual_list`.
  Source: `CMakeLists.txt:12,23-65`. Verification: `.\recipe.ps1 test`.
- **FR-30** The repository shall carry the repo-quality-template tooling at lab requirements revision 4.0.0 with
  the exceptions listed in `README.md`; here `recipe.ps1 test` adds an exact count of 4 exception lines allowed in
  `include/book.hpp` and `include/statistics.hpp`. Source: `README.md`, `recipe.ps1:32-35,126-143`.
  Verification: `.\recipe.ps1 test`.

### 3.11 Declared but not implemented

| Claim | Where | What the code does |
| --- | --- | --- |
| `sampleRandomBooks` throws for a count above the database size | `README.md`, `docs/manual.md` | Returns an empty list for an empty database whatever the count (`include/statistics.hpp:80-82`) |
| "Transparent lookup by authors" in the demo | `src/main.cpp:82-84` | Linear `std::find_if`; the transparent functors are not used |
| "Top 3 books by rating" in the demo | `src/main.cpp:78-79` | `LessByRating` puts the three lowest ratings first |
| The course asked for C++26 on GCC 15 | `CMakeLists.txt:4-5` | The build uses C++23 (`CMakeLists.txt:6`). The legacy `linux-default` preset asks for 26 as a cache variable (`CMakePresets.json:18`), but the plain `set` at `CMakeLists.txt:6` shadows it, so 23 is used there too (shadowing reproduced with CMake; the preset itself not run) |
| Run `run_clang_format.sh` before committing | `src/main.cpp:35` | `recipe.ps1 test` runs `clang-format -i`; `docs/install.md` says no gate runs the script |
| Conan supplies GTest to the presets | `CMakePresets.json`, `scripts/build_windows.ps1` | `CMakeLists.txt` takes GoogleTest from FetchContent and has no `find_package(GTest)`. Inference: the Conan package is not used. Not run |

## 4. Interfaces

### 4.1 Library API (namespace `bookdb`)

| Header | Symbols |
| --- | --- |
| `book.hpp` | `Genre`, `GenreFromString`, `StringFromGenre`, `Book`, `std::formatter<Genre>`, `std::formatter<Book>` |
| `concepts.hpp` | `BookContainerLike`, `BookIterator`, `BookSentinel`, `BookPredicate`, `BookComparator` |
| `book_database.hpp` | `BookDatabase`: `EmplaceBack`, `PushBack`, `Clear`, `operator[]`, `at`, `GetBooks`, `GetAuthors`, iteration, `size`, `empty`, `reserve`, `capacity`; `std::formatter<BookDatabase<std::vector<Book>>>` |
| `comparators.hpp` | `comp::LessByAuthor`, `LessByTitle`, `LessByRating`, `LessByGenre`, `LessByYear`, `LessByPopularity`, `GreaterByRating`, `GreaterByPopularity` |
| `filters.hpp` | `YearBetween`, `RatingAbove`, `GenreIs`, `all_of`, `any_of`, `filterBooks` |
| `heterogeneous_lookup.hpp` | `TransparentStringLess`, `TransparentStringEqual`, `TransparentStringHash` |
| `statistics.hpp` | `buildAuthorHistogramFlat`, `calculateGenreRatings`, `calculateAverageRating`, `sampleRandomBooks`, `getTopNBy`, `std::formatter<flat_map<...>>` |
| `flat_map_compat.hpp` | `flat_map<Key, T, Compare, KeyContainer, MappedContainer>` |

### 4.2 Command line

| Command | Arguments | Effect | Source |
| --- | --- | --- | --- |
| `.\recipe.ps1 <verb>` | `build`, `install`, `uninstall`, `clean`, `run`, `publish`, `info`, `test`; extra arguments go to `run` | See `docs/README.md` | `recipe.ps1:1-7,88-145` |
| `build\BookDB.exe` | none read | Prints the demo to `stdout`, exit 0 | `src/main.cpp:28,87` |
| `build\BookDB_tests.exe` | GoogleTest flags | Runs the cases | `tests/main.cpp`, `docs/manual.md` |

## 5. Data

| Entity | Fields / shape | Storage |
| --- | --- | --- |
| `Book` | `author` view, `title`, `year`, `genre`, `rating`, `read_count` | In the database's container, in memory (`include/book.hpp:67-84`) |
| Author set | `std::unordered_set<std::string>`, one entry per distinct author | In memory; the books' `author` views point into it (`include/book_database.hpp:28,80`) |
| Query results | `std::vector<std::reference_wrapper<const Book>>`, the histogram's `flat_map` with `std::string_view` keys | Refer into the database; valid while it lives (`docs/manual.md`). The genre-rating `flat_map<Genre, double>` holds values |

No file format is read or written by the library or the demo. Copying a `BookDatabase` uses the implicit copy
constructor (`include/book_database.hpp:15-81`), so the copy's `author` views keep pointing into the source's
author set (reproduced, see `docs/README.md`).

## 6. Non-functional requirements

- **NFR-1** The code shall compile as C++23 without compiler extensions. Source: `CMakeLists.txt:6-8`.
- **NFR-2** Non-MSVC builds shall treat warnings as errors with `-Wall -Wextra -Wpedantic -Werror`; MSVC builds
  read sources as UTF-8 (`/utf-8`). Source: `CMakeLists.txt:14-21`.
- **NFR-3** `src/main.cpp` and `tests/*.cpp`, with the headers they include from `include/`
  (`HeaderFilterRegex`), shall pass clang-tidy groups `clang-diagnostic`, `clang-analyzer`, `bugprone` (but
  `easily-swappable-parameters`), `performance`, `portability`, all as errors. Source: `.clang-tidy`,
  `recipe.ps1:135-136`.
- **NFR-4** Sources shall be formatted by `.clang-format` (LLVM base, 120 columns, 4-space indent). Source:
  `.clang-format`, `recipe.ps1:138-139`. `recipe.ps1 test` reformats in place and does not fail on unformatted
  code; the Gitea workflows fail it with `git diff --exit-code` after the recipe
  (`.gitea/workflows/propose.yaml:146-148`).
- **NFR-5** `include` and `src` shall contain no lock or synchronisation primitive. Source: `recipe.ps1:140`.
- **NFR-6** `include` and `src` shall contain no `throw`, `try` or `catch` except exactly 4 lines in
  `include/book.hpp` and `include/statistics.hpp`. Source: `recipe.ps1:32-35,141-142`.
- **NFR-7** The library shall depend on the standard library only and need no installation. Source:
  `README.md`, `docs/install.md`, `CMakeLists.txt:23-24`.
- **NFR-8** The GoogleTest download shall be pinned by archive URL and SHA-256. Source: `CMakeLists.txt:33-41`.

## 7. Constraints and dependencies

| Dependency | Version | Source |
| --- | --- | --- |
| C++ standard | 23 | `CMakeLists.txt:6` |
| CMake | 3.28 or newer | `CMakeLists.txt:1`, `docs/install.md` |
| mingw g++ | 13 or newer | `docs/install.md` |
| Ninja, LLVM `clang-tidy`/`clang-format` | not pinned | `docs/install.md`, `recipe.ps1:52,58` |
| GoogleTest | 1.13.0 (gmock off) | `CMakeLists.txt:37,41` |
| Python with pytest | not pinned | `CMakeLists.txt:62` |
| Legacy route: Conan `gtest/1.13.0`, `cmake/3.30.0`; dev container Ubuntu with g++-15 | as listed | `conanfile.py:12-13`, `.devcontainer/Dockerfile` |

There is no lock file. Licence: MIT, copyright Yandex Practicum (`LICENSE`).

## 8. Acceptance

1. `.\recipe.ps1 test` exits 0, read from `$LASTEXITCODE` directly after the call (`CLAUDE.md`, rule 3). This
   covers FR-1 to FR-24 and FR-26 where a test is named, FR-29, FR-30, NFR-1 to NFR-3, NFR-5 and NFR-6. NFR-4
   holds when `git diff --exit-code` is clean right after it.
2. `build\BookDB_tests.exe --gtest_list_tests` lists the 25 cases of `tests/*.cpp`.
3. `.\recipe.ps1 run` prints the sections named in FR-28 and exits 0.
4. Requirements marked "no test" (FR-14, FR-25, FR-27, FR-28 and the untested branches in FR-3, FR-4, FR-10,
   FR-23, FR-24) are checked by reading the output of step 3 or the code.

## 9. Open questions

- Who uses the library besides the course submission, and does any lab project take it through `publish`
  (`$Needs` of other projects is not visible here)?
- Is `RatingAbove` meant to be inclusive (`>=`)? The name says "above"; the demo text says "≥ 4.5".
- Should `sampleRandomBooks` throw for an empty database and a non-zero count, as `README.md` states?
- Are the swapped title/author and the lowest-rated "Top 3" in the demo to be kept? `docs/manual.md` keeps the
  swap "as the course's example does"; the "Top 3" is not discussed.
- Is copying a `BookDatabase` meant to be supported, given that the copy's author views point at the source?
- Should `filters.hpp` work when included from more than one translation unit (its three non-inline functions
  then collide at link time)?
- Should `BookDatabase` support containers without `reserve` and random access (for example `std::list`), which
  `BookContainerLike` admits?
- Are `CMakePresets.json`, `conanfile.py`, `scripts/build_windows.ps1` and `.devcontainer/` still supported?
  Partly answered: `docs/install.md` calls them what the course and the old GitHub route left, and says no gate
  runs them. Whether they are meant to keep working is not stated.
- No performance or size targets are stated anywhere.
- Have the Gitea workflows run? `CLAUDE.md` says branch protection stays at stage 1 until `propose` and
  `release-gate` have each run once.
- The lab requirements link and issues #8-#16 in `README.md` point to a Gitea server at a loopback address and
  cannot be checked from this repository.
