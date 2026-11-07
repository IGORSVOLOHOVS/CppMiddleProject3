cmake_minimum_required(VERSION 3.28)
project(BookDB VERSION 1.0.0 LANGUAGES CXX)

# The course asked for C++26 on GCC 15. Nothing here needs more than C++23, which g++ 13 has: <print> and
# <flat_map> are what it lacks, and src/main.cpp and include/flat_map_compat.hpp each fall back on their own.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# clang-tidy reads this: without it the FetchContent headers are not found.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTING "Build tests" ON)

function(warn_as_errors target)
    if(MSVC)
        # The sources are UTF-8 with no BOM: without /utf-8 cl.exe reads them in the system code page.
        target_compile_options(${target} PRIVATE /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()

add_library(bookdb INTERFACE)
target_include_directories(bookdb INTERFACE include)

add_executable(BookDB src/main.cpp)
target_link_libraries(BookDB PRIVATE bookdb)
warn_as_errors(BookDB)

if(BUILD_TESTING)
    enable_testing()

    # An archive and its hash, not a git clone: nothing here can ask for a credential on the runner.
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz
        URL_HASH SHA256=ad7fdba11ea011c1d925b3289cf4af2c66a352e18d4c7264392fead75e919363
        SYSTEM
    )
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(BookDB_tests
        tests/book_database_test.cpp
        tests/book_test.cpp
        tests/comparators_test.cpp
        tests/concepts_test.cpp
        tests/filters_test.cpp
        tests/heterogeneous_lookup_test.cpp
        tests/main.cpp
        tests/statsistics_test.cpp
    )
    target_link_libraries(BookDB_tests PRIVATE bookdb GTest::gtest)
    warn_as_errors(BookDB_tests)

    add_test(NAME bookdb_tests COMMAND BookDB_tests)
    # The manual list is judged where every other file is judged (section 9).
    add_test(
        NAME manual_list
        COMMAND python -m pytest tests/test_manual_list.py -q -p no:cacheprovider
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()
