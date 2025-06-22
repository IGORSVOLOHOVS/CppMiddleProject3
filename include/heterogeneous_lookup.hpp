#pragma once

#include <string>
#include <string_view>
#include <functional>

#include "book.hpp"

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    bool operator()(const Book& lhs, const Book& rhs) const { return lhs.author < rhs.author; }
    bool operator()(std::string_view lhs_sv, const Book& rhs) const { return lhs_sv < rhs.author; }
    bool operator()(const Book& lhs, std::string_view rhs_sv) const { return lhs.author < rhs_sv; }
    bool operator()(std::string_view lhs_sv, std::string_view rhs_sv) const { return lhs_sv < rhs_sv; }
};

struct TransparentStringEqual {
    using is_transparent = void;

    bool operator()(const Book& lhs, const Book& rhs) const { return lhs.author == rhs.author; }
    bool operator()(std::string_view lhs_sv, const Book& rhs) const { return lhs_sv == rhs.author; }
    bool operator()(const Book& lhs, std::string_view rhs_sv) const { return lhs.author == rhs_sv; }
    bool operator()(std::string_view lhs_sv, std::string_view rhs_sv) const { return lhs_sv == rhs_sv; }
};

struct TransparentStringHash {
    using is_transparent = void;

    size_t operator()(const Book& key) const { return std::hash<std::string_view>{}(key.author); }
    size_t operator()(std::string_view author) const { return std::hash<std::string_view>{}(author); }
};

}  // namespace bookdb
