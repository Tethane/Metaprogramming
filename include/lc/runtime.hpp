#pragma once

#include <array>

#include "std.hpp"

namespace lc {

template<typename Term>
struct to_int;

template<int N>
struct to_int<Nat<N>> : Nat<N> {};

template<typename Term>
inline constexpr int to_int_v = to_int<Term>::value;

template<typename Term>
struct to_bool;

template<bool B>
struct to_bool<Bool<B>> : Bool<B> {};

template<typename Term>
inline constexpr bool to_bool_v = to_bool<Term>::value;

template<typename Term>
struct to_array;

template<int... Ns>
struct to_array<List<Nat<Ns>...>> {
    static constexpr std::array<int, sizeof...(Ns)> value = {Ns...};
};

template<typename Term>
inline constexpr auto to_array_v = to_array<Term>::value;

} // namespace lc
