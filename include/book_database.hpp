#pragma once

#include <print>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    using value_type = typename BookContainer::value_type;
    using size_type = typename BookContainer::size_type;
    using difference_type = typename BookContainer::difference_type;
    using reference = typename BookContainer::reference;
    using const_reference = typename BookContainer::const_reference;
    using pointer = typename BookContainer::pointer;
    using const_pointer = typename BookContainer::const_pointer;
    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;

    using AuthorContainer = std::unordered_set<std::string>;

    BookDatabase() = default;

    BookDatabase(std::initializer_list<Book> books_init) {
        books_.reserve(books_init.size());
        for (const auto& book_val : books_init) {
            auto [author_it, inserted] = authors_.insert(std::string(book_val.author));
            books_.emplace_back(*author_it, book_val.title, book_val.year, book_val.genre, book_val.rating, book_val.read_count);
        }
    }
    
    iterator begin() { return books_.begin(); }
    iterator end() { return books_.end(); }
    const_iterator begin() const { return books_.cbegin(); }
    const_iterator end() const { return books_.cend(); }
    const_iterator cbegin() const { return books_.cbegin(); }
    const_iterator cend() const { return books_.cend(); }

    size_type size() const { return books_.size(); }
    bool empty() const { return books_.empty(); }
    void reserve(size_type new_cap) { books_.reserve(new_cap); }
    size_type capacity() const { return books_.capacity(); }

    void EmplaceBack(std::string_view author, std::string title, int year, Genre genre, double rating, int read_count) {
        auto [author_it, inserted] = authors_.emplace(author);
        books_.emplace_back(*author_it, std::move(title), year, genre, rating, read_count);
    }

    void PushBack(const Book& book) {
        auto [author_it, inserted] = authors_.insert(std::string(book.author));
        books_.push_back({*author_it, book.title, book.year, book.genre, book.rating, book.read_count});
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    reference operator[](size_type pos) { return books_[pos]; }
    const_reference operator[](size_type pos) const { return books_[pos]; }
    reference at(size_type pos) { return books_.at(pos); }
    const_reference at(size_type pos) const { return books_.at(pos); }

    const BookContainer& GetBooks() const { return books_; }
    const AuthorContainer& GetAuthors() const { return authors_; }
    BookContainer& GetBooks() { return books_; }
    AuthorContainer& GetAuthors() { return authors_; }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
