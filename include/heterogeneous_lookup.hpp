#pragma once

#include "book.hpp"
#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;
    bool operator()(const Book &l, const Book &r) const { return std::less<>()(l, r); }
    bool operator()(const Book &l, std::string_view r) const { return std::less<>()(l, r); }
    bool operator()(std::string_view l, const Book &r) const { return std::less<>()(l, r); }
};

struct TransparentStringEqual {
    using is_transparent = void;
    bool operator()(const Book &l, const Book &r) const { return l == r; }
    bool operator()(std::string l, const Book &r) const { return l == r.author; }
    bool operator()(const Book &l, std::string r) const { return this->operator()(r, l); }
};

struct TransparentStringHash {
    using is_transparent = void;
    auto operator()(const Book &b) const { return std::hash<std::string_view>{}(b.author); }
    auto operator()(std::string l) const { return std::hash<std::string_view>{}(l); }
};

}  // namespace bookdb
