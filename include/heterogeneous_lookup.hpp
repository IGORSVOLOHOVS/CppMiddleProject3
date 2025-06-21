#pragma once

#include <string>
#include <string_view>
#include <unordered_set>

#include "book.hpp"

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;  // поддержка гетерогенного поиска

    bool operator()(const Book &lhs, const Book &rhs) const { 
        return lhs < rhs; 
    }

    bool operator()(std::string_view title, const Book &rhs) const { 
        return title < rhs.title; 
    }
    bool operator()(const Book &lhs, std::string_view title) const { 
        return this->operator()(title, lhs.title);
    }
};

struct TransparentStringEqual {
    using is_transparent = void;  // поддержка гетерогенного поиска

    bool operator()(const Book &lhs, const Book &rhs) const { 
        return lhs == rhs; 
    }

    bool operator()(std::string_view title, const Book &rhs) const { 
        return title == rhs.title; 
    }
    bool operator()(const Book &lhs, std::string_view title) const { 
        return this->operator()(title, lhs.title);
    }
};

struct TransparentStringHash {
    using is_transparent = void; 
    
    std::size_t operator()(const Book &key) const { 
        return std::hash<std::string_view>{}(key.title); 
    }

    std::size_t operator()(std::string_view title) const { 
        return std::hash<std::string_view>{}(title); 
    }
};

}  // namespace bookdb
