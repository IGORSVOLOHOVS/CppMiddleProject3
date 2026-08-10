#pragma once

//
// Зачем этот заголовок нужен.
//
// Проект опирается на std::flat_map (C++23, P0429). В dev-контейнере стоит
// GCC 15 / libstdc++ 15, где этот контейнер есть, поэтому на Linux ничего не
// меняется. А MSVC STL из Visual Studio 2022 (toolset 14.44, cl 19.44)
// заголовок <flat_map> до сих пор не поставляет — именно на нём нативная
// сборка под Windows и разваливалась:
//   include/book.hpp(6): fatal error C1083: Cannot open include file: 'flat_map'
//
// Поэтому здесь объявлен bookdb::flat_map:
//   - если стандартная библиотека умеет std::flat_map, это просто его алиас,
//     то есть на Linux собирается ровно тот же самый код, что и раньше;
//   - если не умеет, подключается компактная замена на отсортированном
//     std::vector<std::pair<Key, T>> с тем же наблюдаемым поведением для той
//     части интерфейса, которой пользуется проект.
//
// Замена намеренно не претендует на полную реализацию стандарта: она покрывает
// используемые операции (lower_bound, find, emplace, emplace_hint, operator[],
// обход, size/empty/clear) и сохраняет сигнатуру шаблона из пяти параметров,
// чтобы частичная специализация std::formatter в statistics.hpp выглядела
// одинаково на обеих платформах.
//

#include <version>

#if __has_include(<flat_map>)
#include <flat_map>
#endif

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

#ifdef __cpp_lib_flat_map

namespace bookdb {

template <class Key, class T, class Compare = std::less<Key>, class KeyContainer = std::vector<Key>,
          class MappedContainer = std::vector<T>>
using flat_map = std::flat_map<Key, T, Compare, KeyContainer, MappedContainer>;

}  // namespace bookdb

#else

namespace bookdb {

// KeyContainer / MappedContainer присутствуют только ради совместимости
// сигнатуры со std::flat_map: хранилище здесь одно, из пар ключ-значение.
template <class Key, class T, class Compare = std::less<Key>, class KeyContainer = std::vector<Key>,
          class MappedContainer = std::vector<T>>
class flat_map {
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<Key, T>;
    using key_compare = Compare;

private:
    using storage_type = std::vector<value_type>;

public:
    using size_type = typename storage_type::size_type;
    using difference_type = typename storage_type::difference_type;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;

    flat_map() = default;
    explicit flat_map(const key_compare &comp) : compare_(comp) {}

    iterator begin() noexcept { return entries_.begin(); }
    iterator end() noexcept { return entries_.end(); }
    const_iterator begin() const noexcept { return entries_.begin(); }
    const_iterator end() const noexcept { return entries_.end(); }
    const_iterator cbegin() const noexcept { return entries_.cbegin(); }
    const_iterator cend() const noexcept { return entries_.cend(); }

    size_type size() const noexcept { return entries_.size(); }
    bool empty() const noexcept { return entries_.empty(); }
    void clear() noexcept { entries_.clear(); }

    const key_compare &key_comp() const noexcept { return compare_; }

    // Гетерогенный поиск: probe может быть любого типа, который понимает
    // Compare (у проекта это TransparentStringLess со std::string_view).
    template <class K>
    iterator lower_bound(const K &probe) {
        return std::lower_bound(entries_.begin(), entries_.end(), probe, EntryLess{&compare_});
    }

    template <class K>
    const_iterator lower_bound(const K &probe) const {
        return std::lower_bound(entries_.begin(), entries_.end(), probe, EntryLess{&compare_});
    }

    template <class K>
    iterator find(const K &probe) {
        const auto it = lower_bound(probe);
        return (it != end() && !compare_(probe, it->first)) ? it : end();
    }

    template <class K>
    const_iterator find(const K &probe) const {
        const auto it = lower_bound(probe);
        return (it != end() && !compare_(probe, it->first)) ? it : end();
    }

    mapped_type &operator[](const key_type &key) {
        const auto it = lower_bound(key);
        if (it != end() && !compare_(key, it->first)) {
            return it->second;
        }
        return entries_.emplace(it, key, mapped_type{})->second;
    }

    template <class... Args>
    std::pair<iterator, bool> emplace(Args &&...args) {
        value_type entry(std::forward<Args>(args)...);
        const auto it = lower_bound(entry.first);
        if (it != end() && !compare_(entry.first, it->first)) {
            return {it, false};
        }
        return {entries_.insert(it, std::move(entry)), true};
    }

    // Подсказка проверяется, а не принимается на веру: неверный hint в
    // стандарте лишь теряет производительность, но не ломает порядок.
    template <class... Args>
    iterator emplace_hint(const_iterator /*hint*/, Args &&...args) {
        return emplace(std::forward<Args>(args)...).first;
    }

private:
    struct EntryLess {
        const key_compare *comp;

        template <class K>
        bool operator()(const value_type &entry, const K &probe) const {
            return (*comp)(entry.first, probe);
        }
    };

    storage_type entries_;
    key_compare compare_{};
};

}  // namespace bookdb

#endif  // __cpp_lib_flat_map
