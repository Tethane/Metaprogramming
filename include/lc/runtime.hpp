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

template<auto Storage>
struct bigint_string_view_storage {
    inline static constexpr auto normalized = detail::normalize_bigint_storage(Storage);
    inline static constexpr std::size_t length = normalized.size + (normalized.negative ? 1 : 0);
    inline static constexpr auto storage = [] {
        std::array<char, bigint_capacity + 2> out{};
        std::size_t cursor = 0;
        if (normalized.negative) {
            out[cursor++] = '-';
        }
        for (std::size_t i = 0; i < normalized.size; ++i) {
            out[cursor++] = normalized.digits[i];
        }
        out[cursor] = '\0';
        return out;
    }();
    inline static constexpr std::string_view value{storage.data(), length};
};

template<typename Term>
struct to_bigint_string_view;

template<auto Storage>
struct to_bigint_string_view<BigInt<Storage>> : bigint_string_view_storage<Storage> {};

template<typename Term>
inline constexpr std::string_view to_bigint_string_view_v = to_bigint_string_view<Term>::value;

template<auto Storage>
struct decimal_string_view_storage {
private:
    inline static constexpr auto normalized = detail::normalize_decimal_storage(Storage);

    static consteval std::size_t compute_length() {
        const std::size_t sign = normalized.negative ? 1u : 0u;
        if (normalized.scale <= 0) {
            return sign + normalized.size;
        }
        const std::size_t scale = static_cast<std::size_t>(normalized.scale);
        if (scale >= normalized.size) {
            return sign + 2u + (scale - normalized.size) + normalized.size;
        }
        return sign + normalized.size + 1u;
    }

public:
    inline static constexpr std::size_t length = compute_length();
    inline static constexpr auto storage = [] {
        std::array<char, decimal_capacity + 4> out{};
        std::size_t cursor = 0;
        if (normalized.negative) {
            out[cursor++] = '-';
        }

        if (normalized.scale <= 0) {
            for (std::size_t i = 0; i < normalized.size; ++i) {
                out[cursor++] = normalized.digits[i];
            }
        } else {
            const std::size_t scale = static_cast<std::size_t>(normalized.scale);
            if (scale >= normalized.size) {
                out[cursor++] = '0';
                out[cursor++] = '.';
                for (std::size_t i = 0; i < scale - normalized.size; ++i) {
                    out[cursor++] = '0';
                }
                for (std::size_t i = 0; i < normalized.size; ++i) {
                    out[cursor++] = normalized.digits[i];
                }
            } else {
                const std::size_t integer_digits = normalized.size - scale;
                for (std::size_t i = 0; i < integer_digits; ++i) {
                    out[cursor++] = normalized.digits[i];
                }
                out[cursor++] = '.';
                for (std::size_t i = integer_digits; i < normalized.size; ++i) {
                    out[cursor++] = normalized.digits[i];
                }
            }
        }

        out[cursor] = '\0';
        return out;
    }();
    inline static constexpr std::string_view value{storage.data(), length};
};

template<typename Term>
struct to_decimal_string_view;

template<auto Storage>
struct to_decimal_string_view<Decimal<Storage>> : decimal_string_view_storage<Storage> {};

template<typename Term>
inline constexpr std::string_view to_decimal_string_view_v = to_decimal_string_view<Term>::value;

template<typename Term>
struct to_rational_string_view;

template<typename Num, typename Den>
struct to_rational_string_view<Rational<Num, Den>> {
private:
    inline static constexpr auto numerator = to_bigint_string_view<Num>::value;
    inline static constexpr auto denominator = to_bigint_string_view<Den>::value;
    inline static constexpr auto storage = [] {
        std::array<char, (bigint_capacity * 2) + 4> out{};
        std::size_t cursor = 0;
        for (char ch : numerator) {
            out[cursor++] = ch;
        }
        out[cursor++] = '/';
        for (char ch : denominator) {
            out[cursor++] = ch;
        }
        out[cursor] = '\0';
        return out;
    }();

public:
    inline static constexpr std::string_view value{storage.data(), numerator.size() + 1 + denominator.size()};
};

template<typename Term>
inline constexpr std::string_view to_rational_string_view_v = to_rational_string_view<Term>::value;

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

template<int... Ns>
struct to_array<Vector<Nat<Ns>...>> {
    static constexpr std::array<int, sizeof...(Ns)> value = {Ns...};
};

template<int... Ns>
struct to_array<Vector<Int<Ns>...>> {
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

template<typename... Rows>
struct to_matrix<Matrix<Rows...>> {
    static constexpr auto value = std::array{to_array<Rows>::value...};
};

template<typename Term>
inline constexpr auto to_matrix_v = to_matrix<Term>::value;

template<typename Stats>
inline constexpr int reduction_count_v = Stats::reductions;

template<typename Stats>
inline constexpr int node_count_v = Stats::nodes;

template<typename Stats>
inline constexpr int approximation_count_v = Stats::approximations;

} // namespace lc
