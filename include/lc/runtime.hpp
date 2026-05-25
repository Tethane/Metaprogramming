#pragma once

#include <array>
#include <string_view>

#include "std.hpp"

namespace lc {

template<typename Term>
struct to_int;

template<int N>
struct to_int<Nat<N>> : Nat<N> {};

template<int N>
struct to_int<Int<N>> : Int<N> {};

template<typename Term>
inline constexpr int to_int_v = to_int<Term>::value;

template<typename Term>
struct to_bool;

template<bool B>
struct to_bool<Bool<B>> : Bool<B> {};

template<typename Term>
inline constexpr bool to_bool_v = to_bool<Term>::value;

template<typename Term>
struct to_string_view;

template<char... Chars>
struct to_string_view<String<Chars...>> {
    inline static constexpr char storage[sizeof...(Chars) + 1] = {Chars..., '\0'};
    inline static constexpr std::string_view value{storage, sizeof...(Chars)};
};

template<typename Term>
inline constexpr std::string_view to_string_view_v = to_string_view<Term>::value;

template<typename Term>
struct to_array;

template<int... Ns>
struct to_array<List<Nat<Ns>...>> {
    static constexpr std::array<int, sizeof...(Ns)> value = {Ns...};
};

template<int... Ns>
struct to_array<List<Int<Ns>...>> {
    static constexpr std::array<int, sizeof...(Ns)> value = {Ns...};
};

template<int... Ns>
struct to_array<Set<Nat<Ns>...>> {
    static constexpr std::array<int, sizeof...(Ns)> value = {Ns...};
};

template<int... Ns>
struct to_array<Set<Int<Ns>...>> {
    static constexpr std::array<int, sizeof...(Ns)> value = {Ns...};
};

template<typename Term>
inline constexpr auto to_array_v = to_array<Term>::value;

template<typename Term>
struct to_matrix;

template<typename... Rows>
struct to_matrix<List<Rows...>> {
    static constexpr auto value = std::array{to_array<Rows>::value...};
};

template<typename Term>
inline constexpr auto to_matrix_v = to_matrix<Term>::value;

} // namespace lc
