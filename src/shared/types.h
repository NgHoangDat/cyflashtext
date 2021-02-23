#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <tuple>
#include <regex>

namespace types {
    template <typename T>
    using Vec = std::vector<T>;

    template <typename K, typename V>
    using Map = std::unordered_map<K, V>;

    template <typename T>
    using Set = std::unordered_set<T>;

    template <typename T, std::size_t N>
    using Array = std::array<T, N>;

    using String = std::string;

    using Code = std::uint32_t;

    using Regex = std::regex;

    template <typename... Args>
    using Tuple = std::tuple<Args...>;
}