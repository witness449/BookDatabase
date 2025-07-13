#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;
    template <typename T>
    bool operator()(const T &a, std::string_view b) const {
        return std::less<>()(a, b);  // Используем стандартную функцию сравнения, адаптированную к типам
    }
};

struct TransparentStringEqual {
    using is_transparent = void;
    template <typename T>
    friend auto operator==(const T &lhs, std::string_view rhs) {
        return lhs == rhs;
    };
    template <typename T>
    friend auto operator==(std::string_view lhs, const T &rhs) {
        return rhs == lhs;
    };
};

struct TransparentStringHash {
    using is_transparent = void;
    /*template <typename T>
    friend auto operator==(const T & lhs, std::string_view rhs){return lhs == std::hash<std::string_view>(rhs){}; };
    template <typename T>
    friend auto operator==(std::string_view lhs, const T& rhs){return rhs == lhs; };*/
};

}  // namespace bookdb
