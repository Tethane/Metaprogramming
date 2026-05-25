#pragma once

#include <cstddef>
#include <utility>

#include "core.hpp"

namespace lc {

struct If {};
struct Not {};
struct And {};
struct Or {};

struct Succ {};
struct Pred {};
struct Add {};
struct Sub {};
struct Mul {};
struct Div {};
struct Mod {};
struct Pow {};
struct Abs {};
struct Eq {};
struct Lt {};
struct Lte {};
struct Gt {};
struct Gte {};
struct IsZero {};

struct Cons {};
struct Head {};
struct Tail {};
struct IsEmpty {};
struct Concat {};
struct Reverse {};
struct Length {};
struct Range {};
struct Map {};
struct Filter {};
struct Foldl {};
struct Foldr {};
struct Sum {};
struct Product {};
struct Any {};
struct All {};
struct Sieve {};
struct TwoSum {};
struct MaxSubarraySum {};
struct ThreeSum {};
struct StringConcat {};
struct StringLength {};
struct StringEq {};
struct StringContains {};
struct StringStartsWith {};
struct StringTake {};
struct StringDrop {};
struct SetInsert {};
struct SetContains {};
struct SetErase {};
struct SetUnion {};
struct SetIntersection {};
struct SetSize {};
struct MapInsert {};
struct MapContainsKey {};
struct MapFind {};
struct MapErase {};
struct MapSize {};
struct Sqrt {};
struct Exp {};
struct Log {};
struct Sin {};
struct Cos {};
struct Tan {};
struct Asin {};
struct Acos {};
struct Atan {};
struct VectorCtor {};
struct MatrixCtor {};
struct ComplexCtor {};
struct VecAdd {};
struct VecSub {};
struct VecScale {};
struct Dot {};
struct Norm {};
struct NormalizeVector {};
struct MatAdd {};
struct MatSub {};
struct MatScale {};
struct MatVecMul {};
struct MatMul {};
struct Transpose {};
struct Determinant {};
struct Inverse {};
struct Conjugate {};
struct NormSquared {};
struct Magnitude {};
struct Argument {};
struct Mean {};
struct Median {};
struct Mode {};
struct Variance {};
struct StdDev {};
struct Minimum {};
struct Maximum {};
struct StatRange {};
struct Covariance {};
struct Correlation {};

namespace detail {

constexpr int pow_constexpr(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

template<typename Number>
struct ValueOf;

template<int N>
struct ValueOf<Nat<N>> : Nat<N> {};

template<int N>
struct ValueOf<Int<N>> : Int<N> {};

template<typename T>
inline constexpr int value_of_v = ValueOf<T>::value;

constexpr bool bigint_is_zero(const bigint_storage& value) {
    return value.size == 1 && value.digits[0] == '0';
}

constexpr bigint_storage normalize_bigint_storage(bigint_storage value) {
    std::size_t first = 0;
    while (first + 1 < value.size && value.digits[first] == '0') {
        ++first;
    }

    if (first != 0) {
        for (std::size_t i = 0; i + first < value.size; ++i) {
            value.digits[i] = value.digits[i + first];
        }
        value.size -= first;
    }

    if (bigint_is_zero(value)) {
        value.negative = false;
    }
    for (std::size_t i = value.size; i < bigint_capacity; ++i) {
        value.digits[i] = '\0';
    }
    return value;
}

consteval bigint_storage make_bigint_storage_from_int(long long raw) {
    bigint_storage value{};
    unsigned long long magnitude = static_cast<unsigned long long>(raw < 0 ? -raw : raw);
    value.negative = raw < 0;
    value.size = 0;

    if (magnitude == 0) {
        value.size = 1;
        value.digits[0] = '0';
        value.negative = false;
        return value;
    }

    char reversed[bigint_capacity] = {};
    while (magnitude != 0) {
        reversed[value.size++] = static_cast<char>('0' + (magnitude % 10ULL));
        magnitude /= 10ULL;
    }

    for (std::size_t i = 0; i < value.size; ++i) {
        value.digits[i] = reversed[value.size - 1 - i];
    }
    return value;
}

constexpr int digit_value(char ch) {
    return ch - '0';
}

consteval bigint_storage make_bigint_storage_from_chars(const char* chars, std::size_t size) {
    bigint_storage value{};
    std::size_t offset = 0;
    if (size != 0 && chars[0] == '-') {
        value.negative = true;
        offset = 1;
    } else if (size != 0 && chars[0] == '+') {
        offset = 1;
    }

    value.size = (offset < size) ? (size - offset) : 1;
    if (value.size == 0) {
        value.size = 1;
        value.digits[0] = '0';
        value.negative = false;
        return value;
    }

    for (std::size_t i = 0; i < value.size; ++i) {
        value.digits[i] = (offset + i < size) ? chars[offset + i] : '0';
    }
    return normalize_bigint_storage(value);
}

template<fixed_string Digits>
inline constexpr auto bigint_literal_storage_v = make_bigint_storage_from_chars(Digits.value, Digits.size);

template<fixed_string Digits>
using BigIntLiteral_t = BigInt<bigint_literal_storage_v<Digits>>;

template<long long N>
using BigIntFromInt_t = BigInt<make_bigint_storage_from_int(N)>;

template<typename T>
struct ToBigInt;

template<int N>
struct ToBigInt<Nat<N>> {
    using type = BigIntFromInt_t<N>;
};

template<int N>
struct ToBigInt<Int<N>> {
    using type = BigIntFromInt_t<N>;
};

template<auto Storage>
struct ToBigInt<BigInt<Storage>> {
    using type = BigInt<normalize_bigint_storage(Storage)>;
};

template<typename T>
using ToBigInt_t = typename ToBigInt<T>::type;

template<typename T>
struct IsIntegerLike : std::false_type {};

template<int N>
struct IsIntegerLike<Nat<N>> : std::true_type {};

template<int N>
struct IsIntegerLike<Int<N>> : std::true_type {};

template<auto Storage>
struct IsIntegerLike<BigInt<Storage>> : std::true_type {};

template<typename T>
inline constexpr bool is_integer_like_v = IsIntegerLike<T>::value;

template<typename T>
struct IsRationalLike : std::false_type {};

template<typename Num, typename Den>
struct IsRationalLike<Rational<Num, Den>> : std::true_type {};

template<typename T>
inline constexpr bool is_rational_like_v = IsRationalLike<T>::value;

template<auto Left, auto Right>
constexpr int compare_abs_bigint_storage() {
    constexpr auto lhs = normalize_bigint_storage(Left);
    constexpr auto rhs = normalize_bigint_storage(Right);

    if constexpr (lhs.size < rhs.size) {
        return -1;
    } else if constexpr (lhs.size > rhs.size) {
        return 1;
    } else {
        for (std::size_t i = 0; i < lhs.size; ++i) {
            if (lhs.digits[i] < rhs.digits[i]) {
                return -1;
            }
            if (lhs.digits[i] > rhs.digits[i]) {
                return 1;
            }
        }
        return 0;
    }
}

constexpr int compare_abs_bigint_storage(const bigint_storage& lhs, const bigint_storage& rhs) {
    if (lhs.size < rhs.size) {
        return -1;
    }
    if (lhs.size > rhs.size) {
        return 1;
    }
    for (std::size_t i = 0; i < lhs.size; ++i) {
        if (lhs.digits[i] < rhs.digits[i]) {
            return -1;
        }
        if (lhs.digits[i] > rhs.digits[i]) {
            return 1;
        }
    }
    return 0;
}

constexpr int compare_bigint_storage(const bigint_storage& lhs_in, const bigint_storage& rhs_in) {
    const auto lhs = normalize_bigint_storage(lhs_in);
    const auto rhs = normalize_bigint_storage(rhs_in);
    if (lhs.negative != rhs.negative) {
        return lhs.negative ? -1 : 1;
    }
    const int abs_cmp = compare_abs_bigint_storage(lhs, rhs);
    return lhs.negative ? -abs_cmp : abs_cmp;
}

constexpr bigint_storage add_abs_bigint_storage(const bigint_storage& lhs_in, const bigint_storage& rhs_in) {
    const auto lhs = normalize_bigint_storage(lhs_in);
    const auto rhs = normalize_bigint_storage(rhs_in);
    bigint_storage out{};
    out.negative = false;
    std::size_t li = lhs.size;
    std::size_t ri = rhs.size;
    std::size_t count = 0;
    int carry = 0;
    char reversed[bigint_capacity] = {};

    while (li > 0 || ri > 0 || carry != 0) {
        const int l = (li > 0) ? digit_value(lhs.digits[--li]) : 0;
        const int r = (ri > 0) ? digit_value(rhs.digits[--ri]) : 0;
        const int sum = l + r + carry;
        reversed[count++] = static_cast<char>('0' + (sum % 10));
        carry = sum / 10;
    }

    out.size = count;
    for (std::size_t i = 0; i < count; ++i) {
        out.digits[i] = reversed[count - 1 - i];
    }
    return normalize_bigint_storage(out);
}

constexpr bigint_storage sub_abs_bigint_storage(const bigint_storage& lhs_in, const bigint_storage& rhs_in) {
    const auto lhs = normalize_bigint_storage(lhs_in);
    const auto rhs = normalize_bigint_storage(rhs_in);
    bigint_storage out{};
    out.negative = false;
    std::size_t li = lhs.size;
    std::size_t ri = rhs.size;
    std::size_t count = 0;
    int borrow = 0;
    char reversed[bigint_capacity] = {};

    while (li > 0) {
        int l = digit_value(lhs.digits[--li]) - borrow;
        const int r = (ri > 0) ? digit_value(rhs.digits[--ri]) : 0;
        if (l < r) {
            l += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        reversed[count++] = static_cast<char>('0' + (l - r));
    }

    out.size = count;
    for (std::size_t i = 0; i < count; ++i) {
        out.digits[i] = reversed[count - 1 - i];
    }
    return normalize_bigint_storage(out);
}

constexpr bigint_storage negate_bigint_storage(bigint_storage value) {
    value = normalize_bigint_storage(value);
    if (!bigint_is_zero(value)) {
        value.negative = !value.negative;
    }
    return value;
}

constexpr bigint_storage add_bigint_storage(const bigint_storage& lhs_in, const bigint_storage& rhs_in) {
    const auto lhs = normalize_bigint_storage(lhs_in);
    const auto rhs = normalize_bigint_storage(rhs_in);
    if (lhs.negative == rhs.negative) {
        auto out = add_abs_bigint_storage(lhs, rhs);
        out.negative = lhs.negative;
        return normalize_bigint_storage(out);
    }

    const int cmp = compare_abs_bigint_storage(lhs, rhs);
    if (cmp == 0) {
        return make_bigint_storage_from_int(0);
    }
    if (cmp > 0) {
        auto out = sub_abs_bigint_storage(lhs, rhs);
        out.negative = lhs.negative;
        return normalize_bigint_storage(out);
    }

    auto out = sub_abs_bigint_storage(rhs, lhs);
    out.negative = rhs.negative;
    return normalize_bigint_storage(out);
}

constexpr bigint_storage sub_bigint_storage(const bigint_storage& lhs, const bigint_storage& rhs) {
    return add_bigint_storage(lhs, negate_bigint_storage(rhs));
}

constexpr bigint_storage mul_bigint_storage(const bigint_storage& lhs_in, const bigint_storage& rhs_in) {
    const auto lhs = normalize_bigint_storage(lhs_in);
    const auto rhs = normalize_bigint_storage(rhs_in);
    if (bigint_is_zero(lhs) || bigint_is_zero(rhs)) {
        return make_bigint_storage_from_int(0);
    }

    int accum[bigint_capacity] = {};
    for (std::size_t li = 0; li < lhs.size; ++li) {
        const int l = digit_value(lhs.digits[lhs.size - 1 - li]);
        for (std::size_t ri = 0; ri < rhs.size; ++ri) {
            const int r = digit_value(rhs.digits[rhs.size - 1 - ri]);
            accum[li + ri] += l * r;
        }
    }

    for (std::size_t i = 0; i + 1 < bigint_capacity; ++i) {
        accum[i + 1] += accum[i] / 10;
        accum[i] %= 10;
    }

    std::size_t used = bigint_capacity;
    while (used > 1 && accum[used - 1] == 0) {
        --used;
    }

    bigint_storage out{};
    out.negative = lhs.negative != rhs.negative;
    out.size = used;
    for (std::size_t i = 0; i < used; ++i) {
        out.digits[i] = static_cast<char>('0' + accum[used - 1 - i]);
    }
    return normalize_bigint_storage(out);
}

constexpr bigint_storage append_bigint_digit(const bigint_storage& lhs_in, int digit) {
    const auto lhs = normalize_bigint_storage(lhs_in);
    if (bigint_is_zero(lhs)) {
        bigint_storage out{};
        out.size = 1;
        out.digits[0] = static_cast<char>('0' + digit);
        out.negative = false;
        return out;
    }

    bigint_storage out{};
    out.negative = false;
    out.size = lhs.size + 1;
    for (std::size_t i = 0; i < lhs.size; ++i) {
        out.digits[i] = lhs.digits[i];
    }
    out.digits[lhs.size] = static_cast<char>('0' + digit);
    return normalize_bigint_storage(out);
}

struct bigint_divmod_result {
    bigint_storage quotient;
    bigint_storage remainder;
};

constexpr bigint_divmod_result divmod_abs_bigint_storage(const bigint_storage& lhs_in, const bigint_storage& rhs_in) {
    const auto lhs = normalize_bigint_storage(lhs_in);
    const auto rhs = normalize_bigint_storage(rhs_in);
    if (bigint_is_zero(rhs)) {
        return {make_bigint_storage_from_int(0), lhs};
    }

    bigint_storage quotient{};
    quotient.negative = false;
    quotient.size = lhs.size;
    bigint_storage remainder = make_bigint_storage_from_int(0);

    for (std::size_t i = 0; i < lhs.size; ++i) {
        remainder = append_bigint_digit(remainder, digit_value(lhs.digits[i]));
        int qdigit = 0;
        while (compare_abs_bigint_storage(remainder, rhs) >= 0) {
            remainder = sub_abs_bigint_storage(remainder, rhs);
            ++qdigit;
        }
        quotient.digits[i] = static_cast<char>('0' + qdigit);
    }

    quotient = normalize_bigint_storage(quotient);
    remainder = normalize_bigint_storage(remainder);
    return {quotient, remainder};
}

constexpr bigint_storage abs_bigint_storage(bigint_storage value) {
    value = normalize_bigint_storage(value);
    value.negative = false;
    return value;
}

constexpr bigint_storage gcd_bigint_storage(bigint_storage lhs, bigint_storage rhs) {
    lhs = abs_bigint_storage(lhs);
    rhs = abs_bigint_storage(rhs);
    while (!bigint_is_zero(rhs)) {
        const auto divmod = divmod_abs_bigint_storage(lhs, rhs);
        lhs = rhs;
        rhs = divmod.remainder;
    }
    return normalize_bigint_storage(lhs);
}

template<typename T>
struct BigIntStorageOf;

template<int N>
struct BigIntStorageOf<Nat<N>> {
    inline static constexpr auto value = make_bigint_storage_from_int(N);
};

template<int N>
struct BigIntStorageOf<Int<N>> {
    inline static constexpr auto value = make_bigint_storage_from_int(N);
};

template<auto Storage>
struct BigIntStorageOf<BigInt<Storage>> {
    inline static constexpr auto value = normalize_bigint_storage(Storage);
};

template<typename T>
inline constexpr auto bigint_storage_of_v = BigIntStorageOf<T>::value;

template<auto Storage>
using NormalizedBigInt_t = BigInt<normalize_bigint_storage(Storage)>;

template<typename Left, typename Right>
struct BigIntAddResult;

template<typename Left, typename Right>
struct BigIntSubResult;

template<typename Left, typename Right>
struct BigIntMulResult;

template<typename Left, typename Right>
struct BigIntDivResult;

template<typename Left, typename Right>
struct BigIntModResult;

template<typename Num, typename Den>
struct NormalizeRational {
private:
    inline static constexpr auto raw_num = bigint_storage_of_v<Num>;
    inline static constexpr auto raw_den = bigint_storage_of_v<Den>;
    inline static constexpr auto gcd = gcd_bigint_storage(raw_num, raw_den);
    inline static constexpr auto reduced_num = divmod_abs_bigint_storage(abs_bigint_storage(raw_num), gcd).quotient;
    inline static constexpr auto reduced_den = divmod_abs_bigint_storage(abs_bigint_storage(raw_den), gcd).quotient;
    inline static constexpr auto signed_num = [] {
        auto out = reduced_num;
        out.negative = raw_num.negative != raw_den.negative;
        return normalize_bigint_storage(out);
    }();

public:
    using type = Rational<NormalizedBigInt_t<signed_num>, NormalizedBigInt_t<reduced_den>>;
};

template<typename Num, typename Den>
using NormalizeRational_t = typename NormalizeRational<Num, Den>::type;

template<typename T>
struct ToRational;

template<int N>
struct ToRational<Nat<N>> {
    using type = Rational<BigIntFromInt_t<N>, BigIntFromInt_t<1>>;
};

template<int N>
struct ToRational<Int<N>> {
    using type = Rational<BigIntFromInt_t<N>, BigIntFromInt_t<1>>;
};

template<auto Storage>
struct ToRational<BigInt<Storage>> {
    using type = Rational<NormalizedBigInt_t<Storage>, BigIntFromInt_t<1>>;
};

template<typename Num, typename Den>
struct ToRational<Rational<Num, Den>> {
    using type = NormalizeRational_t<Num, Den>;
};

template<typename T>
using ToRational_t = typename ToRational<T>::type;

template<typename T>
struct RationalParts;

template<typename Num, typename Den>
struct RationalParts<Rational<Num, Den>> {
    using numerator = Num;
    using denominator = Den;
};

template<typename T>
struct IsDecimalLike : std::false_type {};

template<auto Storage>
struct IsDecimalLike<Decimal<Storage>> : std::true_type {};

template<typename T>
inline constexpr bool is_decimal_like_v = IsDecimalLike<T>::value;

template<typename T>
struct IsSymbolicReal : std::false_type {};

template<typename Tag>
struct IsSymbolicReal<Irrational<Tag>> : std::true_type {};

template<typename Op, typename... Args>
struct IsSymbolicReal<RealExpr<Op, Args...>> : std::true_type {};

template<typename T>
inline constexpr bool is_symbolic_real_v = IsSymbolicReal<T>::value;

template<typename T>
struct IsScalarReal
    : std::bool_constant<
          is_integer_like_v<T> ||
          is_rational_like_v<T> ||
          is_decimal_like_v<T> ||
          is_symbolic_real_v<T>> {};

template<typename T>
inline constexpr bool is_scalar_real_v = IsScalarReal<T>::value;

template<typename T>
struct IsComplexLike : std::false_type {};

template<typename Real, typename Imag>
struct IsComplexLike<Complex<Real, Imag>> : std::true_type {};

template<typename T>
inline constexpr bool is_complex_like_v = IsComplexLike<T>::value;

constexpr bool decimal_is_zero(const decimal_storage& value) {
    return value.size == 1 && value.digits[0] == '0';
}

constexpr decimal_storage normalize_decimal_storage(decimal_storage value) {
    while (value.size > 1 && value.scale > 0 && value.digits[value.size - 1] == '0') {
        --value.size;
        --value.scale;
    }

    std::size_t first = 0;
    while (first + 1 < value.size && value.digits[first] == '0') {
        ++first;
    }

    if (first != 0) {
        for (std::size_t i = 0; i + first < value.size; ++i) {
            value.digits[i] = value.digits[i + first];
        }
        value.size -= first;
    }

    if (decimal_is_zero(value)) {
        value.negative = false;
        value.scale = 0;
    }

    for (std::size_t i = value.size; i < decimal_capacity; ++i) {
        value.digits[i] = '\0';
    }
    return value;
}

constexpr long double ld_abs(long double value) {
    return value < 0.0L ? -value : value;
}

constexpr long double pi_ld = 3.141592653589793238462643383279502884L;
constexpr long double e_ld = 2.718281828459045235360287471352662497L;
constexpr long double ln2_ld = 0.693147180559945309417232121458176568L;

constexpr long double reduce_angle(long double x) {
    const long double two_pi = 2.0L * pi_ld;
    while (x > pi_ld) {
        x -= two_pi;
    }
    while (x < -pi_ld) {
        x += two_pi;
    }
    return x;
}

constexpr long double pow_ld(long double base, int exp) {
    long double result = 1.0L;
    const bool negative = exp < 0;
    int count = negative ? -exp : exp;
    for (int i = 0; i < count; ++i) {
        result *= base;
    }
    return negative ? (1.0L / result) : result;
}

constexpr long double sqrt_ld(long double value) {
    if (value <= 0.0L) {
        return 0.0L;
    }
    long double guess = value > 1.0L ? value : 1.0L;
    for (int i = 0; i < 32; ++i) {
        guess = 0.5L * (guess + value / guess);
    }
    return guess;
}

constexpr long double exp_ld(long double value) {
    if (value == 0.0L) {
        return 1.0L;
    }
    if (value < 0.0L) {
        return 1.0L / exp_ld(-value);
    }
    if (value > 1.0L) {
        const long double half = exp_ld(value / 2.0L);
        return half * half;
    }
    long double term = 1.0L;
    long double sum = 1.0L;
    for (int n = 1; n < 48; ++n) {
        term *= value / static_cast<long double>(n);
        sum += term;
    }
    return sum;
}

constexpr long double log_ld(long double value) {
    if (value <= 0.0L) {
        return 0.0L;
    }
    int shifts = 0;
    while (value > 1.5L) {
        value /= 2.0L;
        ++shifts;
    }
    while (value < 0.75L) {
        value *= 2.0L;
        --shifts;
    }
    const long double y = (value - 1.0L) / (value + 1.0L);
    const long double y2 = y * y;
    long double term = y;
    long double sum = 0.0L;
    for (int n = 1; n < 80; n += 2) {
        sum += term / static_cast<long double>(n);
        term *= y2;
    }
    return 2.0L * sum + static_cast<long double>(shifts) * ln2_ld;
}

constexpr long double sin_ld(long double value) {
    value = reduce_angle(value);
    long double term = value;
    long double sum = value;
    for (int n = 1; n < 28; ++n) {
        const long double denom = static_cast<long double>((2 * n) * (2 * n + 1));
        term *= -value * value / denom;
        sum += term;
    }
    return sum;
}

constexpr long double cos_ld(long double value) {
    value = reduce_angle(value);
    long double term = 1.0L;
    long double sum = 1.0L;
    for (int n = 1; n < 28; ++n) {
        const long double denom = static_cast<long double>((2 * n - 1) * (2 * n));
        term *= -value * value / denom;
        sum += term;
    }
    return sum;
}

constexpr long double atan_ld(long double value) {
    if (value < 0.0L) {
        return -atan_ld(-value);
    }
    if (value > 1.0L) {
        return (pi_ld / 2.0L) - atan_ld(1.0L / value);
    }
    long double term = value;
    long double sum = value;
    const long double value_sq = value * value;
    for (int n = 1; n < 80; ++n) {
        term *= -value_sq;
        sum += term / static_cast<long double>(2 * n + 1);
    }
    return sum;
}

constexpr long double asin_ld(long double value) {
    return atan_ld(value / sqrt_ld(1.0L - value * value));
}

constexpr long double acos_ld(long double value) {
    return (pi_ld / 2.0L) - asin_ld(value);
}

constexpr decimal_storage make_decimal_from_long_double(long double raw, int digits) {
    decimal_storage value{};
    value.negative = raw < 0.0L;
    long double number = ld_abs(raw);
    const int fractional_digits = digits < 0 ? 0 : digits;

    char integer_rev[decimal_capacity] = {};
    std::size_t integer_size = 0;
    unsigned long long integer_part = static_cast<unsigned long long>(number);
    long double fraction = number - static_cast<long double>(integer_part);

    if (integer_part == 0ULL) {
        integer_rev[integer_size++] = '0';
    } else {
        while (integer_part != 0ULL && integer_size < decimal_capacity) {
            integer_rev[integer_size++] = static_cast<char>('0' + (integer_part % 10ULL));
            integer_part /= 10ULL;
        }
    }

    value.size = 0;
    for (std::size_t i = 0; i < integer_size; ++i) {
        value.digits[value.size++] = integer_rev[integer_size - 1 - i];
    }

    for (int i = 0; i < fractional_digits && value.size < decimal_capacity; ++i) {
        fraction *= 10.0L;
        int digit = static_cast<int>(fraction);
        if (digit < 0) {
            digit = 0;
        }
        if (digit > 9) {
            digit = 9;
        }
        value.digits[value.size++] = static_cast<char>('0' + digit);
        fraction -= static_cast<long double>(digit);
    }

    value.scale = fractional_digits;
    return normalize_decimal_storage(value);
}

template<typename T>
struct ApproxLongDouble;

template<int N>
struct ApproxLongDouble<Nat<N>> {
    static constexpr long double value = static_cast<long double>(N);
};

template<int N>
struct ApproxLongDouble<Int<N>> {
    static constexpr long double value = static_cast<long double>(N);
};

template<auto Storage>
struct ApproxLongDouble<BigInt<Storage>> {
    static constexpr long double value = [] {
        const auto normalized = normalize_bigint_storage(Storage);
        long double result = 0.0L;
        for (std::size_t i = 0; i < normalized.size; ++i) {
            result = (result * 10.0L) + static_cast<long double>(digit_value(normalized.digits[i]));
        }
        return normalized.negative ? -result : result;
    }();
};

template<typename Num, typename Den>
struct ApproxLongDouble<Rational<Num, Den>> {
    static constexpr long double value = ApproxLongDouble<Num>::value / ApproxLongDouble<Den>::value;
};

template<auto Storage>
struct ApproxLongDouble<Decimal<Storage>> {
    static constexpr long double value = [] {
        const auto normalized = normalize_decimal_storage(Storage);
        long double result = 0.0L;
        for (std::size_t i = 0; i < normalized.size; ++i) {
            result = (result * 10.0L) + static_cast<long double>(digit_value(normalized.digits[i]));
        }
        result /= pow_ld(10.0L, normalized.scale);
        return normalized.negative ? -result : result;
    }();
};

template<>
struct ApproxLongDouble<Irrational<pi_tag>> {
    static constexpr long double value = pi_ld;
};

template<>
struct ApproxLongDouble<Irrational<e_tag>> {
    static constexpr long double value = e_ld;
};

template<>
struct ApproxLongDouble<Irrational<tau_tag>> {
    static constexpr long double value = 2.0L * pi_ld;
};

template<typename Op, typename... Args>
struct ApproxLongDouble<RealExpr<Op, Args...>>;

template<typename Left, typename Right>
struct ApproxLongDouble<RealExpr<Add, Left, Right>> {
    static constexpr long double value = ApproxLongDouble<Left>::value + ApproxLongDouble<Right>::value;
};

template<typename Left, typename Right>
struct ApproxLongDouble<RealExpr<Sub, Left, Right>> {
    static constexpr long double value = ApproxLongDouble<Left>::value - ApproxLongDouble<Right>::value;
};

template<typename Left, typename Right>
struct ApproxLongDouble<RealExpr<Mul, Left, Right>> {
    static constexpr long double value = ApproxLongDouble<Left>::value * ApproxLongDouble<Right>::value;
};

template<typename Left, typename Right>
struct ApproxLongDouble<RealExpr<Div, Left, Right>> {
    static constexpr long double value = ApproxLongDouble<Left>::value / ApproxLongDouble<Right>::value;
};

template<typename Arg>
struct ApproxLongDouble<RealExpr<Sqrt, Arg>> {
    static constexpr long double value = sqrt_ld(ApproxLongDouble<Arg>::value);
};

template<typename Arg>
struct ApproxLongDouble<RealExpr<Exp, Arg>> {
    static constexpr long double value = exp_ld(ApproxLongDouble<Arg>::value);
};

template<typename Arg>
struct ApproxLongDouble<RealExpr<Log, Arg>> {
    static constexpr long double value = log_ld(ApproxLongDouble<Arg>::value);
};

template<typename Arg>
struct ApproxLongDouble<RealExpr<Sin, Arg>> {
    static constexpr long double value = sin_ld(ApproxLongDouble<Arg>::value);
};

template<typename Arg>
struct ApproxLongDouble<RealExpr<Cos, Arg>> {
    static constexpr long double value = cos_ld(ApproxLongDouble<Arg>::value);
};

template<typename Arg>
struct ApproxLongDouble<RealExpr<Tan, Arg>> {
    static constexpr long double value = sin_ld(ApproxLongDouble<Arg>::value) / cos_ld(ApproxLongDouble<Arg>::value);
};

template<typename Arg>
struct ApproxLongDouble<RealExpr<Asin, Arg>> {
    static constexpr long double value = asin_ld(ApproxLongDouble<Arg>::value);
};

template<typename Arg>
struct ApproxLongDouble<RealExpr<Acos, Arg>> {
    static constexpr long double value = acos_ld(ApproxLongDouble<Arg>::value);
};

template<typename Arg>
struct ApproxLongDouble<RealExpr<Atan, Arg>> {
    static constexpr long double value = atan_ld(ApproxLongDouble<Arg>::value);
};

template<typename T, int Digits>
struct ApproxValue {
    using type = T;
};

template<int N, int Digits>
struct ApproxValue<Nat<N>, Digits> {
    using type = Decimal<make_decimal_from_long_double(static_cast<long double>(N), Digits)>;
};

template<int N, int Digits>
struct ApproxValue<Int<N>, Digits> {
    using type = Decimal<make_decimal_from_long_double(static_cast<long double>(N), Digits)>;
};

template<auto Storage, int Digits>
struct ApproxValue<BigInt<Storage>, Digits> {
    using type = Decimal<make_decimal_from_long_double(ApproxLongDouble<BigInt<Storage>>::value, Digits)>;
};

template<typename Num, typename Den, int Digits>
struct ApproxValue<Rational<Num, Den>, Digits> {
    using type = Decimal<make_decimal_from_long_double(ApproxLongDouble<Rational<Num, Den>>::value, Digits)>;
};

template<auto Storage, int Digits>
struct ApproxValue<Decimal<Storage>, Digits> {
    using type = Decimal<normalize_decimal_storage(Storage)>;
};

template<typename Tag, int Digits>
struct ApproxValue<Irrational<Tag>, Digits> {
    using type = Decimal<make_decimal_from_long_double(ApproxLongDouble<Irrational<Tag>>::value, Digits)>;
};

template<typename Op, typename... Args, int Digits>
struct ApproxValue<RealExpr<Op, Args...>, Digits> {
    using type = Decimal<make_decimal_from_long_double(ApproxLongDouble<RealExpr<Op, Args...>>::value, Digits)>;
};

template<typename Real, typename Imag, int Digits>
struct ApproxValue<Complex<Real, Imag>, Digits> {
    using type = Complex<typename ApproxValue<Real, Digits>::type, typename ApproxValue<Imag, Digits>::type>;
};

template<typename T, int Digits>
using ApproxValue_t = typename ApproxValue<T, Digits>::type;

template<typename T>
struct BigIntFitsLongLong : std::false_type {};

template<int N>
struct BigIntFitsLongLong<Nat<N>> : std::true_type {};

template<int N>
struct BigIntFitsLongLong<Int<N>> : std::true_type {};

template<auto Storage>
struct BigIntFitsLongLong<BigInt<Storage>>
    : std::bool_constant<(normalize_bigint_storage(Storage).size < 18)> {};

template<typename T>
inline constexpr bool bigint_fits_ll_v = BigIntFitsLongLong<T>::value;

template<typename T>
constexpr long long bigint_to_ll() {
    constexpr auto normalized = bigint_storage_of_v<T>;
    long long result = 0;
    for (std::size_t i = 0; i < normalized.size; ++i) {
        result = result * 10LL + static_cast<long long>(digit_value(normalized.digits[i]));
    }
    return normalized.negative ? -result : result;
}

template<typename T>
struct VectorSize;

template<typename... Elems>
struct VectorSize<Vector<Elems...>> : std::integral_constant<std::size_t, sizeof...(Elems)> {};

template<std::size_t Index, typename VectorT>
struct VectorAt;

template<std::size_t Index, typename Head, typename... Tail>
struct VectorAt<Index, Vector<Head, Tail...>> : VectorAt<Index - 1, Vector<Tail...>> {};

template<typename Head, typename... Tail>
struct VectorAt<0, Vector<Head, Tail...>> {
    using type = Head;
};

template<std::size_t Index, typename VectorT>
using VectorAt_t = typename VectorAt<Index, VectorT>::type;

template<typename... Ts>
struct VectorPushFront;

template<typename T, typename... Ts>
struct VectorPushFront<T, Vector<Ts...>> {
    using type = Vector<T, Ts...>;
};

template<typename T, typename VectorT>
using VectorPushFront_t = typename VectorPushFront<T, VectorT>::type;

template<std::size_t N, std::size_t M>
constexpr bool string_starts_with_chars(const char (&text)[N], const char (&prefix)[M]) {
    if constexpr (M == 1) {
        return true;
    } else if constexpr (N < M) {
        return false;
    } else {
        for (std::size_t i = 0; i < M - 1; ++i) {
            if (text[i] != prefix[i]) {
                return false;
            }
        }
        return true;
    }
}

template<std::size_t N, std::size_t M>
constexpr bool string_contains_chars(const char (&text)[N], const char (&needle)[M]) {
    if constexpr (M == 1) {
        return true;
    } else if constexpr (N < M) {
        return false;
    } else {
        for (std::size_t offset = 0; offset + (M - 1) <= (N - 1); ++offset) {
            bool match = true;
            for (std::size_t i = 0; i < M - 1; ++i) {
                if (text[offset + i] != needle[i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return true;
            }
        }
        return false;
    }
}

template<typename StrT, typename AccT = String<>>
struct StringReverseAcc;

template<char... AccChars>
struct StringReverseAcc<String<>, String<AccChars...>> {
    using type = String<AccChars...>;
};

template<char First, char... Rest, char... AccChars>
struct StringReverseAcc<String<First, Rest...>, String<AccChars...>> {
    using type = typename StringReverseAcc<String<Rest...>, String<First, AccChars...>>::type;
};

template<typename StrT>
using StringReverse_t = typename StringReverseAcc<StrT>::type;

template<typename StrT, typename TailT>
struct StringConcatT;

template<char... Left, char... Right>
struct StringConcatT<String<Left...>, String<Right...>> {
    using type = String<Left..., Right...>;
};

template<typename Left, typename Right>
using StringConcat_t = typename StringConcatT<Left, Right>::type;

template<int Count, typename StrT, bool Done = (Count == 0)>
struct StringTakeImpl;

template<int Count, bool Done>
struct StringTakeImpl<Count, String<>, Done> {
    using type = String<>;
};

template<int Count, char First, char... Rest>
struct StringTakeImpl<Count, String<First, Rest...>, true> {
    using type = String<>;
};

template<int Count, char First, char... Rest>
struct StringTakeImpl<Count, String<First, Rest...>, false> {
    using type = StringConcat_t<String<First>, typename StringTakeImpl<Count - 1, String<Rest...>>::type>;
};

template<int Count, typename StrT>
using StringTake_t = typename StringTakeImpl<Count, StrT>::type;

template<int Count, typename StrT, bool Done = (Count == 0)>
struct StringDropImpl;

template<int Count, bool Done>
struct StringDropImpl<Count, String<>, Done> {
    using type = String<>;
};

template<int Count, char... Chars>
struct StringDropImpl<Count, String<Chars...>, true> {
    using type = String<Chars...>;
};

template<int Count, char First, char... Rest>
struct StringDropImpl<Count, String<First, Rest...>, false> {
    using type = typename StringDropImpl<Count - 1, String<Rest...>>::type;
};

template<int Count, typename StrT>
using StringDrop_t = typename StringDropImpl<Count, StrT>::type;

template<typename Item, typename SetT>
struct SetHas;

template<typename Item>
struct SetHas<Item, Set<>> : std::false_type {};

template<typename Item, typename Head, typename... Tail>
struct SetHas<Item, Set<Head, Tail...>>
    : std::bool_constant<IsSame<Item, Head>::value || SetHas<Item, Set<Tail...>>::value> {};

template<typename Item, typename SetT>
struct SetInsertT;

template<typename Item, typename... Items>
struct SetInsertT<Item, Set<Items...>> {
    using type = IfType_t<SetHas<Item, Set<Items...>>::value, Set<Items...>, Set<Items..., Item>>;
};

template<typename Item, typename SetT>
using SetInsert_t = typename SetInsertT<Item, SetT>::type;

template<typename Item, typename SetT>
struct SetPrependT;

template<typename Item, typename... Items>
struct SetPrependT<Item, Set<Items...>> {
    using type = Set<Item, Items...>;
};

template<typename Item, typename SetT>
using SetPrepend_t = typename SetPrependT<Item, SetT>::type;

template<typename Item, typename SetT>
struct SetEraseT;

template<typename Item>
struct SetEraseT<Item, Set<>> {
    using type = Set<>;
};

template<typename Item, typename Head, typename... Tail>
struct SetEraseT<Item, Set<Head, Tail...>> {
private:
    using tail = typename SetEraseT<Item, Set<Tail...>>::type;

public:
    using type = IfType_t<IsSame<Item, Head>::value, tail, SetPrepend_t<Head, tail>>;
};

template<typename Item, typename SetT>
using SetErase_t = typename SetEraseT<Item, SetT>::type;

template<typename LeftSet, typename RightSet>
struct SetUnionT;

template<typename... LeftItems>
struct SetUnionT<Set<LeftItems...>, Set<>> {
    using type = Set<LeftItems...>;
};

template<typename... LeftItems, typename Item, typename... Rest>
struct SetUnionT<Set<LeftItems...>, Set<Item, Rest...>> {
    using type = typename SetUnionT<SetInsert_t<Item, Set<LeftItems...>>, Set<Rest...>>::type;
};

template<typename LeftSet, typename RightSet>
using SetUnion_t = typename SetUnionT<LeftSet, RightSet>::type;

template<typename LeftSet, typename RightSet, typename Acc = Set<>>
struct SetIntersectionAcc;

template<typename RightSet, typename... AccItems>
struct SetIntersectionAcc<Set<>, RightSet, Set<AccItems...>> {
    using type = Set<AccItems...>;
};

template<typename Head, typename... Tail, typename RightSet, typename... AccItems>
struct SetIntersectionAcc<Set<Head, Tail...>, RightSet, Set<AccItems...>> {
private:
    using next_acc = IfType_t<SetHas<Head, RightSet>::value, SetInsert_t<Head, Set<AccItems...>>, Set<AccItems...>>;

public:
    using type = typename SetIntersectionAcc<Set<Tail...>, RightSet, next_acc>::type;
};

template<typename LeftSet, typename RightSet>
using SetIntersection_t = typename SetIntersectionAcc<LeftSet, RightSet>::type;

template<typename Key, typename MapT>
struct AssocFindT;

template<typename Key>
struct AssocFindT<Key, AssocMap<>> {
    using type = None;
};

template<typename Key, typename Value, typename... Rest>
struct AssocFindT<Key, AssocMap<Entry<Key, Value>, Rest...>> {
    using type = Value;
};

template<typename Key, typename EntryT, typename... Rest>
struct AssocFindT<Key, AssocMap<EntryT, Rest...>> {
    using type = typename AssocFindT<Key, AssocMap<Rest...>>::type;
};

template<typename Key, typename MapT>
using AssocFind_t = typename AssocFindT<Key, MapT>::type;

template<typename Key, typename Value, typename MapT>
struct AssocInsertT;

template<typename Key, typename Value>
struct AssocInsertT<Key, Value, AssocMap<>> {
    using type = AssocMap<Entry<Key, Value>>;
};

template<typename Key, typename Value, typename ExistingValue, typename... Rest>
struct AssocInsertT<Key, Value, AssocMap<Entry<Key, ExistingValue>, Rest...>> {
    using type = AssocMap<Entry<Key, Value>, Rest...>;
};

template<typename Key, typename Value, typename EntryT, typename... Rest>
struct AssocInsertT<Key, Value, AssocMap<EntryT, Rest...>> {
private:
    using tail = typename AssocInsertT<Key, Value, AssocMap<Rest...>>::type;

public:
    template<typename... TailEntries>
    static AssocMap<EntryT, TailEntries...> rebuild(AssocMap<TailEntries...>);

    using type = decltype(rebuild(tail{}));
};

template<typename Key, typename Value, typename MapT>
using AssocInsert_t = typename AssocInsertT<Key, Value, MapT>::type;

template<typename Key, typename MapT>
struct AssocEraseT;

template<typename Key>
struct AssocEraseT<Key, AssocMap<>> {
    using type = AssocMap<>;
};

template<typename Key, typename Value, typename... Rest>
struct AssocEraseT<Key, AssocMap<Entry<Key, Value>, Rest...>> {
    using type = AssocMap<Rest...>;
};

template<typename Key, typename EntryT, typename... Rest>
struct AssocEraseT<Key, AssocMap<EntryT, Rest...>> {
private:
    using tail = typename AssocEraseT<Key, AssocMap<Rest...>>::type;

public:
    template<typename... TailEntries>
    static AssocMap<EntryT, TailEntries...> rebuild(AssocMap<TailEntries...>);

    using type = decltype(rebuild(tail{}));
};

template<typename Key, typename MapT>
using AssocErase_t = typename AssocEraseT<Key, MapT>::type;

template<typename T, typename ListT>
struct ListPushFront;

template<typename T, typename... Ts>
struct ListPushFront<T, List<Ts...>> {
    using type = List<T, Ts...>;
};

template<typename T, typename ListT>
using ListPushFront_t = typename ListPushFront<T, ListT>::type;

template<typename ListT, typename T>
struct ListPushBack;

template<typename... Ts, typename T>
struct ListPushBack<List<Ts...>, T> {
    using type = List<Ts..., T>;
};

template<typename ListT, typename T>
using ListPushBack_t = typename ListPushBack<ListT, T>::type;

template<int Begin, int End, bool Done = (Begin > End)>
struct RangeList;

template<int Begin, int End>
struct RangeList<Begin, End, true> {
    using type = List<>;
};

template<int Begin, int End>
struct RangeList<Begin, End, false> {
    using type = ListPushFront_t<Nat<Begin>, typename RangeList<Begin + 1, End>::type>;
};

template<int Begin, int End>
using RangeList_t = typename RangeList<Begin, End>::type;

template<typename Func, typename ListT>
struct MapList;

template<typename Func>
struct MapList<Func, List<>> {
    using type = List<>;
};

template<typename Func, typename Item, typename... Rest>
struct MapList<Func, List<Item, Rest...>> {
    using mapped_item = Normalize_t<Apply_t<Func, Item>, 4096>;
    using type = ListPushFront_t<mapped_item, typename MapList<Func, List<Rest...>>::type>;
};

template<typename Func, typename ListT>
using MapList_t = typename MapList<Func, ListT>::type;

template<typename Predicate, typename ListT>
struct FilterList;

template<typename Predicate>
struct FilterList<Predicate, List<>> {
    using type = List<>;
};

template<typename Predicate, typename Item, typename... Rest>
struct FilterList<Predicate, List<Item, Rest...>> {
    using keep = Normalize_t<Apply_t<Predicate, Item>, 4096>;
    using tail = typename FilterList<Predicate, List<Rest...>>::type;
    using type = IfType_t<keep::value, ListPushFront_t<Item, tail>, tail>;
};

template<typename Predicate, typename ListT>
using FilterList_t = typename FilterList<Predicate, ListT>::type;

template<typename Func, typename Acc, typename ListT>
struct FoldlList;

template<typename Func, typename Acc>
struct FoldlList<Func, Acc, List<>> {
    using type = Acc;
};

template<typename Func, typename Acc, typename Item, typename... Rest>
struct FoldlList<Func, Acc, List<Item, Rest...>> {
    using next_acc = Normalize_t<Apply_t<Func, Acc, Item>, 4096>;
    using type = typename FoldlList<Func, next_acc, List<Rest...>>::type;
};

template<typename Func, typename Acc, typename ListT>
using FoldlList_t = typename FoldlList<Func, Acc, ListT>::type;

template<typename Func, typename Acc, typename ListT>
struct FoldrList;

template<typename Func, typename Acc>
struct FoldrList<Func, Acc, List<>> {
    using type = Acc;
};

template<typename Func, typename Acc, typename Item, typename... Rest>
struct FoldrList<Func, Acc, List<Item, Rest...>> {
    using rest_value = typename FoldrList<Func, Acc, List<Rest...>>::type;
    using type = Normalize_t<Apply_t<Func, Item, rest_value>, 4096>;
};

template<typename Func, typename Acc, typename ListT>
using FoldrList_t = typename FoldrList<Func, Acc, ListT>::type;

template<typename Left, typename Right>
struct ConcatLists;

template<typename... LeftItems, typename... RightItems>
struct ConcatLists<List<LeftItems...>, List<RightItems...>> {
    using type = List<LeftItems..., RightItems...>;
};

template<typename Left, typename Right>
using ConcatLists_t = typename ConcatLists<Left, Right>::type;

template<typename Remaining, typename Acc = List<>>
struct ReverseAcc;

template<typename... AccItems>
struct ReverseAcc<List<>, List<AccItems...>> {
    using type = List<AccItems...>;
};

template<typename Item, typename... Rest, typename... AccItems>
struct ReverseAcc<List<Item, Rest...>, List<AccItems...>> {
    using type = typename ReverseAcc<List<Rest...>, List<Item, AccItems...>>::type;
};

template<typename ListT>
using ReverseList_t = typename ReverseAcc<ListT>::type;

template<typename ListT>
struct SumList;

template<>
struct SumList<List<>> {
    using type = Nat<0>;
};

template<int N, typename... Rest>
struct SumList<List<Nat<N>, Rest...>> {
    using rest = typename SumList<List<Rest...>>::type;
    using type = Nat<N + rest::value>;
};

template<typename ListT>
using SumList_t = typename SumList<ListT>::type;

template<typename ListT>
struct ProductList;

template<>
struct ProductList<List<>> {
    using type = Nat<1>;
};

template<int N, typename... Rest>
struct ProductList<List<Nat<N>, Rest...>> {
    using rest = typename ProductList<List<Rest...>>::type;
    using type = Nat<N * rest::value>;
};

template<typename ListT>
using ProductList_t = typename ProductList<ListT>::type;

template<typename ListT>
struct AnyList;

template<>
struct AnyList<List<>> {
    using type = Bool<false>;
};

template<bool B, typename... Rest>
struct AnyList<List<Bool<B>, Rest...>> {
    using rest = typename AnyList<List<Rest...>>::type;
    using type = Bool<B || rest::value>;
};

template<typename ListT>
using AnyList_t = typename AnyList<ListT>::type;

template<typename ListT>
struct AllList;

template<>
struct AllList<List<>> {
    using type = Bool<true>;
};

template<bool B, typename... Rest>
struct AllList<List<Bool<B>, Rest...>> {
    using rest = typename AllList<List<Rest...>>::type;
    using type = Bool<B && rest::value>;
};

template<typename ListT>
using AllList_t = typename AllList<ListT>::type;

template<typename T, typename ListT>
struct ListContains;

template<typename T>
struct ListContains<T, List<>> : std::false_type {};

template<typename T, typename Head, typename... Tail>
struct ListContains<T, List<Head, Tail...>>
    : std::bool_constant<IsSame<T, Head>::value || ListContains<T, List<Tail...>>::value> {};

template<typename Left, typename Right>
struct UniqueConcatLists;

template<typename... LeftItems>
struct UniqueConcatLists<List<LeftItems...>, List<>> {
    using type = List<LeftItems...>;
};

template<typename... LeftItems, typename Item, typename... Rest>
struct UniqueConcatLists<List<LeftItems...>, List<Item, Rest...>> {
private:
    using appended = IfType_t<
        ListContains<Item, List<LeftItems...>>::value,
        List<LeftItems...>,
        List<LeftItems..., Item>
    >;

public:
    using type = typename UniqueConcatLists<appended, List<Rest...>>::type;
};

template<typename Left, typename Right>
using UniqueConcatLists_t = typename UniqueConcatLists<Left, Right>::type;

template<int Needed, int Index, typename ListT>
struct FindValueIndex;

template<int Needed, int Index>
struct FindValueIndex<Needed, Index, List<>> {
    using type = List<>;
};

template<bool Match, int Needed, int Index, typename Item, typename... Rest>
struct FindValueIndexStep;

template<int Needed, int Index, typename Item, typename... Rest>
struct FindValueIndexStep<true, Needed, Index, Item, Rest...> {
    using type = List<Nat<Index>>;
};

template<int Needed, int Index, typename Item, typename... Rest>
struct FindValueIndexStep<false, Needed, Index, Item, Rest...> {
    using type = typename FindValueIndex<Needed, Index + 1, List<Rest...>>::type;
};

template<int Needed, int Index, typename Item, typename... Rest>
struct FindValueIndex<Needed, Index, List<Item, Rest...>> {
    using type = typename FindValueIndexStep<
        (value_of_v<Item> == Needed),
        Needed,
        Index,
        Item,
        Rest...
    >::type;
};

template<int Target, int Index, typename ListT>
struct TwoSumList;

template<int Target, int Index>
struct TwoSumList<Target, Index, List<>> {
    using type = List<>;
};

template<typename InnerResult, int CurrentIndex, int Target, int NextIndex, typename RestList>
struct TwoSumListAfterInner;

template<int CurrentIndex, int Target, int NextIndex, typename RestList>
struct TwoSumListAfterInner<List<>, CurrentIndex, Target, NextIndex, RestList> {
    using type = typename TwoSumList<Target, NextIndex, RestList>::type;
};

template<typename MatchIndex, int CurrentIndex, int Target, int NextIndex, typename RestList>
struct TwoSumListAfterInner {
    using type = ListPushFront_t<Nat<CurrentIndex>, MatchIndex>;
};

template<int Target, int Index, typename Item, typename... Rest>
struct TwoSumList<Target, Index, List<Item, Rest...>> {
private:
    using inner = typename FindValueIndex<Target - value_of_v<Item>, Index + 1, List<Rest...>>::type;

public:
    using type = typename TwoSumListAfterInner<inner, Index, Target, Index + 1, List<Rest...>>::type;
};

template<int Target, typename ListT>
using TwoSumList_t = typename TwoSumList<Target, 0, ListT>::type;

template<int CurrentBest, int Running, typename ListT>
struct MaxSubarrayScan;

template<int CurrentBest, int Running>
struct MaxSubarrayScan<CurrentBest, Running, List<>> {
    using type = Int<CurrentBest>;
};

template<int CurrentBest, int Running, typename Item, typename... Rest>
struct MaxSubarrayScan<CurrentBest, Running, List<Item, Rest...>> {
private:
    static constexpr int item_value = value_of_v<Item>;
    static constexpr int next_running = (Running + item_value > item_value) ? (Running + item_value) : item_value;
    static constexpr int next_best = (CurrentBest > next_running) ? CurrentBest : next_running;

public:
    using type = typename MaxSubarrayScan<next_best, next_running, List<Rest...>>::type;
};

template<typename ListT>
struct MaxSubarrayList;

template<>
struct MaxSubarrayList<List<>> {
    using type = Int<0>;
};

template<typename Item, typename... Rest>
struct MaxSubarrayList<List<Item, Rest...>> {
    using type = typename MaxSubarrayScan<value_of_v<Item>, value_of_v<Item>, List<Rest...>>::type;
};

template<typename ListT>
using MaxSubarrayList_t = typename MaxSubarrayList<ListT>::type;

template<int First, int Second, typename ListT>
struct ThirdSumMatches;

template<int First, int Second>
struct ThirdSumMatches<First, Second, List<>> {
    using type = List<>;
};

template<int First, int Second, typename Item, typename... Rest>
struct ThirdSumMatches<First, Second, List<Item, Rest...>> {
private:
    using tail = typename ThirdSumMatches<First, Second, List<Rest...>>::type;
    using triplet = List<Int<First>, Int<Second>, Int<value_of_v<Item>>>;

public:
    using type = IfType_t<
        (First + Second + value_of_v<Item> == 0),
        UniqueConcatLists_t<List<triplet>, tail>,
        tail
    >;
};

template<int First, typename ListT>
struct ThreeSumSecondLoop;

template<int First>
struct ThreeSumSecondLoop<First, List<>> {
    using type = List<>;
};

template<int First, typename Item, typename... Rest>
struct ThreeSumSecondLoop<First, List<Item, Rest...>> {
private:
    using with_second = typename ThirdSumMatches<First, value_of_v<Item>, List<Rest...>>::type;
    using without_second = typename ThreeSumSecondLoop<First, List<Rest...>>::type;

public:
    using type = UniqueConcatLists_t<with_second, without_second>;
};

template<typename ListT>
struct ThreeSumList;

template<>
struct ThreeSumList<List<>> {
    using type = List<>;
};

template<typename Item>
struct ThreeSumList<List<Item>> {
    using type = List<>;
};

template<typename First, typename Second>
struct ThreeSumList<List<First, Second>> {
    using type = List<>;
};

template<typename Item, typename... Rest>
struct ThreeSumList<List<Item, Rest...>> {
private:
    using with_first = typename ThreeSumSecondLoop<value_of_v<Item>, List<Rest...>>::type;
    using without_first = typename ThreeSumList<List<Rest...>>::type;

public:
    using type = UniqueConcatLists_t<with_first, without_first>;
};

template<typename ListT>
using ThreeSumList_t = typename ThreeSumList<ListT>::type;

template<int Prime, typename ListT>
struct RemoveMultiples;

template<int Prime>
struct RemoveMultiples<Prime, List<>> {
    using type = List<>;
};

template<int Prime, int N, typename... Rest>
struct RemoveMultiples<Prime, List<Nat<N>, Rest...>> {
    using tail = typename RemoveMultiples<Prime, List<Rest...>>::type;
    using type = IfType_t<(N % Prime != 0), ListPushFront_t<Nat<N>, tail>, tail>;
};

template<int Prime, typename ListT>
using RemoveMultiples_t = typename RemoveMultiples<Prime, ListT>::type;

template<typename ListT>
struct SieveList;

template<>
struct SieveList<List<>> {
    using type = List<>;
};

template<int Prime, typename... Rest>
struct SieveList<List<Nat<Prime>, Rest...>> {
    using filtered_tail = RemoveMultiples_t<Prime, List<Rest...>>;
    using tail_primes = typename SieveList<filtered_tail>::type;
    using type = ListPushFront_t<Nat<Prime>, tail_primes>;
};

template<typename ListT>
using SieveList_t = typename SieveList<ListT>::type;

template<typename Left, typename Right>
using ScalarAdd_t = Normalize_t<Apply_t<Add, Left, Right>, 4096>;

template<typename Left, typename Right>
using ScalarSub_t = Normalize_t<Apply_t<Sub, Left, Right>, 4096>;

template<typename Left, typename Right>
using ScalarMul_t = Normalize_t<Apply_t<Mul, Left, Right>, 4096>;

template<typename Left, typename Right>
using ScalarDiv_t = Normalize_t<Apply_t<Div, Left, Right>, 4096>;

template<typename Value>
using ScalarSqrt_t = Normalize_t<Apply_t<Sqrt, Value>, 4096>;

template<typename VectorT>
struct VectorSum;

template<>
struct VectorSum<Vector<>> {
    using type = Int<0>;
};

template<typename Head, typename... Tail>
struct VectorSum<Vector<Head, Tail...>> {
    using type = ScalarAdd_t<Head, typename VectorSum<Vector<Tail...>>::type>;
};

template<typename VectorT>
using VectorSum_t = typename VectorSum<VectorT>::type;

template<typename LeftVec, typename RightVec>
struct VectorAddT;

template<>
struct VectorAddT<Vector<>, Vector<>> {
    using type = Vector<>;
};

template<typename LHead, typename... LTail, typename RHead, typename... RTail>
struct VectorAddT<Vector<LHead, LTail...>, Vector<RHead, RTail...>> {
    using type = VectorPushFront_t<
        ScalarAdd_t<LHead, RHead>,
        typename VectorAddT<Vector<LTail...>, Vector<RTail...>>::type>;
};

template<typename LeftVec, typename RightVec>
using VectorAdd_t = typename VectorAddT<LeftVec, RightVec>::type;

template<typename LeftVec, typename RightVec>
struct VectorSubT;

template<>
struct VectorSubT<Vector<>, Vector<>> {
    using type = Vector<>;
};

template<typename LHead, typename... LTail, typename RHead, typename... RTail>
struct VectorSubT<Vector<LHead, LTail...>, Vector<RHead, RTail...>> {
    using type = VectorPushFront_t<
        ScalarSub_t<LHead, RHead>,
        typename VectorSubT<Vector<LTail...>, Vector<RTail...>>::type>;
};

template<typename LeftVec, typename RightVec>
using VectorSub_t = typename VectorSubT<LeftVec, RightVec>::type;

template<typename Scalar, typename VectorT>
struct VectorScaleT;

template<typename Scalar>
struct VectorScaleT<Scalar, Vector<>> {
    using type = Vector<>;
};

template<typename Scalar, typename Head, typename... Tail>
struct VectorScaleT<Scalar, Vector<Head, Tail...>> {
    using type = VectorPushFront_t<
        ScalarMul_t<Scalar, Head>,
        typename VectorScaleT<Scalar, Vector<Tail...>>::type>;
};

template<typename Scalar, typename VectorT>
using VectorScale_t = typename VectorScaleT<Scalar, VectorT>::type;

template<typename LeftVec, typename RightVec>
struct VectorDotT;

template<>
struct VectorDotT<Vector<>, Vector<>> {
    using type = Int<0>;
};

template<typename LHead, typename... LTail, typename RHead, typename... RTail>
struct VectorDotT<Vector<LHead, LTail...>, Vector<RHead, RTail...>> {
    using type = ScalarAdd_t<
        ScalarMul_t<LHead, RHead>,
        typename VectorDotT<Vector<LTail...>, Vector<RTail...>>::type>;
};

template<typename LeftVec, typename RightVec>
using VectorDot_t = typename VectorDotT<LeftVec, RightVec>::type;

template<typename VectorT>
using VectorNorm_t = ScalarSqrt_t<VectorDot_t<VectorT, VectorT>>;

template<typename VectorT, typename NormT>
struct VectorNormalizeT;

template<typename NormT>
struct VectorNormalizeT<Vector<>, NormT> {
    using type = Vector<>;
};

template<typename Head, typename... Tail, typename NormT>
struct VectorNormalizeT<Vector<Head, Tail...>, NormT> {
    using type = VectorPushFront_t<
        ScalarDiv_t<Head, NormT>,
        typename VectorNormalizeT<Vector<Tail...>, NormT>::type>;
};

template<typename VectorT>
using VectorNormalize_t = typename VectorNormalizeT<VectorT, VectorNorm_t<VectorT>>::type;

template<typename Row, typename MatrixT>
struct MatrixPushFront;

template<typename Row, typename... Rows>
struct MatrixPushFront<Row, Matrix<Rows...>> {
    using type = Matrix<Row, Rows...>;
};

template<typename Row, typename MatrixT>
using MatrixPushFront_t = typename MatrixPushFront<Row, MatrixT>::type;

template<typename MatrixT, typename Scalar>
struct MatrixScaleT;

template<typename Scalar>
struct MatrixScaleT<Matrix<>, Scalar> {
    using type = Matrix<>;
};

template<typename Row, typename... RestRows, typename Scalar>
struct MatrixScaleT<Matrix<Row, RestRows...>, Scalar> {
    using type = MatrixPushFront_t<
        VectorScale_t<Scalar, Row>,
        typename MatrixScaleT<Matrix<RestRows...>, Scalar>::type>;
};

template<typename MatrixT, typename Scalar>
using MatrixScale_t = typename MatrixScaleT<MatrixT, Scalar>::type;

template<typename LeftMat, typename RightMat>
struct MatrixAddT;

template<>
struct MatrixAddT<Matrix<>, Matrix<>> {
    using type = Matrix<>;
};

template<typename LRow, typename... LRows, typename RRow, typename... RRows>
struct MatrixAddT<Matrix<LRow, LRows...>, Matrix<RRow, RRows...>> {
    using type = MatrixPushFront_t<
        VectorAdd_t<LRow, RRow>,
        typename MatrixAddT<Matrix<LRows...>, Matrix<RRows...>>::type>;
};

template<typename LeftMat, typename RightMat>
using MatrixAdd_t = typename MatrixAddT<LeftMat, RightMat>::type;

template<typename LeftMat, typename RightMat>
struct MatrixSubT;

template<>
struct MatrixSubT<Matrix<>, Matrix<>> {
    using type = Matrix<>;
};

template<typename LRow, typename... LRows, typename RRow, typename... RRows>
struct MatrixSubT<Matrix<LRow, LRows...>, Matrix<RRow, RRows...>> {
    using type = MatrixPushFront_t<
        VectorSub_t<LRow, RRow>,
        typename MatrixSubT<Matrix<LRows...>, Matrix<RRows...>>::type>;
};

template<typename LeftMat, typename RightMat>
using MatrixSub_t = typename MatrixSubT<LeftMat, RightMat>::type;

template<std::size_t Index, typename MatrixT>
struct MatrixColumn;

template<std::size_t Index>
struct MatrixColumn<Index, Matrix<>> {
    using type = Vector<>;
};

template<std::size_t Index, typename Row, typename... RestRows>
struct MatrixColumn<Index, Matrix<Row, RestRows...>> {
    using type = VectorPushFront_t<
        VectorAt_t<Index, Row>,
        typename MatrixColumn<Index, Matrix<RestRows...>>::type>;
};

template<std::size_t Index, typename MatrixT>
using MatrixColumn_t = typename MatrixColumn<Index, MatrixT>::type;

template<typename MatrixT, typename Indices>
struct MatrixTransposeImpl;

template<typename... Rows, std::size_t... Indices>
struct MatrixTransposeImpl<Matrix<Rows...>, std::index_sequence<Indices...>> {
    using type = Matrix<MatrixColumn_t<Indices, Matrix<Rows...>>...>;
};

template<typename MatrixT>
struct MatrixTranspose;

template<typename FirstRow, typename... RestRows>
struct MatrixTranspose<Matrix<FirstRow, RestRows...>> {
    using type = typename MatrixTransposeImpl<
        Matrix<FirstRow, RestRows...>,
        std::make_index_sequence<VectorSize<FirstRow>::value>>::type;
};

template<typename MatrixT>
using MatrixTranspose_t = typename MatrixTranspose<MatrixT>::type;

template<typename MatrixT, typename VectorT>
struct MatrixVecMulT;

template<typename VectorT>
struct MatrixVecMulT<Matrix<>, VectorT> {
    using type = Vector<>;
};

template<typename Row, typename... RestRows, typename VectorT>
struct MatrixVecMulT<Matrix<Row, RestRows...>, VectorT> {
    using type = VectorPushFront_t<
        VectorDot_t<Row, VectorT>,
        typename MatrixVecMulT<Matrix<RestRows...>, VectorT>::type>;
};

template<typename MatrixT, typename VectorT>
using MatrixVecMul_t = typename MatrixVecMulT<MatrixT, VectorT>::type;

template<typename LeftMat, typename RightMat>
struct MatrixMulT;

template<typename... LeftRows, typename RightMat>
struct MatrixMulT<Matrix<LeftRows...>, RightMat> {
private:
    using transposed = MatrixTranspose_t<RightMat>;

    template<typename Row, typename TMat>
    struct MultiplyRow;

    template<typename Row>
    struct MultiplyRow<Row, Matrix<>> {
        using type = Vector<>;
    };

    template<typename Row, typename Col, typename... RestCols>
    struct MultiplyRow<Row, Matrix<Col, RestCols...>> {
        using type = VectorPushFront_t<
            VectorDot_t<Row, Col>,
            typename MultiplyRow<Row, Matrix<RestCols...>>::type>;
    };

public:
    using type = Matrix<typename MultiplyRow<LeftRows, transposed>::type...>;
};

template<typename LeftMat, typename RightMat>
using MatrixMul_t = typename MatrixMulT<LeftMat, RightMat>::type;

template<typename MatrixT>
struct DeterminantT;

template<typename A, typename B, typename C, typename D>
struct DeterminantT<Matrix<Vector<A, B>, Vector<C, D>>> {
    using type = ScalarSub_t<ScalarMul_t<A, D>, ScalarMul_t<B, C>>;
};

template<typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I>
struct DeterminantT<Matrix<Vector<A, B, C>, Vector<D, E, F>, Vector<G, H, I>>> {
private:
    using term1 = ScalarMul_t<A, ScalarSub_t<ScalarMul_t<E, I>, ScalarMul_t<F, H>>>;
    using term2 = ScalarMul_t<B, ScalarSub_t<ScalarMul_t<D, I>, ScalarMul_t<F, G>>>;
    using term3 = ScalarMul_t<C, ScalarSub_t<ScalarMul_t<D, H>, ScalarMul_t<E, G>>>;

public:
    using type = ScalarAdd_t<ScalarSub_t<term1, term2>, term3>;
};

template<typename MatrixT>
using Determinant_t = typename DeterminantT<MatrixT>::type;

template<typename MatrixT>
struct InverseT;

template<typename A, typename B, typename C, typename D>
struct InverseT<Matrix<Vector<A, B>, Vector<C, D>>> {
private:
    using det = Determinant_t<Matrix<Vector<A, B>, Vector<C, D>>>;

public:
    using type = Matrix<
        Vector<ScalarDiv_t<D, det>, ScalarDiv_t<ScalarMul_t<Int<-1>, B>, det>>,
        Vector<ScalarDiv_t<ScalarMul_t<Int<-1>, C>, det>, ScalarDiv_t<A, det>>
    >;
};

template<typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I>
struct InverseT<Matrix<Vector<A, B, C>, Vector<D, E, F>, Vector<G, H, I>>> {
private:
    using det = Determinant_t<Matrix<Vector<A, B, C>, Vector<D, E, F>, Vector<G, H, I>>>;
    using c11 = ScalarSub_t<ScalarMul_t<E, I>, ScalarMul_t<F, H>>;
    using c12 = ScalarMul_t<Int<-1>, ScalarSub_t<ScalarMul_t<D, I>, ScalarMul_t<F, G>>>;
    using c13 = ScalarSub_t<ScalarMul_t<D, H>, ScalarMul_t<E, G>>;
    using c21 = ScalarMul_t<Int<-1>, ScalarSub_t<ScalarMul_t<B, I>, ScalarMul_t<C, H>>>;
    using c22 = ScalarSub_t<ScalarMul_t<A, I>, ScalarMul_t<C, G>>;
    using c23 = ScalarMul_t<Int<-1>, ScalarSub_t<ScalarMul_t<A, H>, ScalarMul_t<B, G>>>;
    using c31 = ScalarSub_t<ScalarMul_t<B, F>, ScalarMul_t<C, E>>;
    using c32 = ScalarMul_t<Int<-1>, ScalarSub_t<ScalarMul_t<A, F>, ScalarMul_t<C, D>>>;
    using c33 = ScalarSub_t<ScalarMul_t<A, E>, ScalarMul_t<B, D>>;
    using adj = Matrix<Vector<c11, c21, c31>, Vector<c12, c22, c32>, Vector<c13, c23, c33>>;

    template<typename Row>
    struct DivideRow;

    template<typename X, typename Y, typename Z>
    struct DivideRow<Vector<X, Y, Z>> {
        using type = Vector<ScalarDiv_t<X, det>, ScalarDiv_t<Y, det>, ScalarDiv_t<Z, det>>;
    };

public:
    using type = Matrix<
        typename DivideRow<Vector<c11, c21, c31>>::type,
        typename DivideRow<Vector<c12, c22, c32>>::type,
        typename DivideRow<Vector<c13, c23, c33>>::type
    >;
};

template<typename MatrixT>
using Inverse_t = typename InverseT<MatrixT>::type;

template<typename VectorT>
using Mean_t = ScalarDiv_t<VectorSum_t<VectorT>, Nat<static_cast<int>(VectorSize<VectorT>::value)>>;

template<typename VectorT, typename MeanValue>
struct CenterVectorT;

template<typename MeanValue>
struct CenterVectorT<Vector<>, MeanValue> {
    using type = Vector<>;
};

template<typename Head, typename... Tail, typename MeanValue>
struct CenterVectorT<Vector<Head, Tail...>, MeanValue> {
    using type = VectorPushFront_t<
        ScalarSub_t<Head, MeanValue>,
        typename CenterVectorT<Vector<Tail...>, MeanValue>::type>;
};

template<typename VectorT>
using Variance_t = ScalarDiv_t<
    VectorDot_t<typename CenterVectorT<VectorT, Mean_t<VectorT>>::type, typename CenterVectorT<VectorT, Mean_t<VectorT>>::type>,
    Nat<static_cast<int>(VectorSize<VectorT>::value)>>;

template<typename VectorT>
using StdDev_t = ScalarSqrt_t<Variance_t<VectorT>>;

template<typename VectorT>
struct MinimumT;

template<typename Head>
struct MinimumT<Vector<Head>> {
    using type = Head;
};

template<typename Head, typename Next, typename... Tail>
struct MinimumT<Vector<Head, Next, Tail...>> {
private:
    using smaller = Normalize_t<Apply_t<Lt, Head, Next>, 4096>;

public:
    using type = typename MinimumT<Vector<IfType_t<smaller::value, Head, Next>, Tail...>>::type;
};

template<typename VectorT>
using Minimum_t = typename MinimumT<VectorT>::type;

template<typename VectorT>
struct MaximumT;

template<typename Head>
struct MaximumT<Vector<Head>> {
    using type = Head;
};

template<typename Head, typename Next, typename... Tail>
struct MaximumT<Vector<Head, Next, Tail...>> {
private:
    using larger = Normalize_t<Apply_t<Gt, Head, Next>, 4096>;

public:
    using type = typename MaximumT<Vector<IfType_t<larger::value, Head, Next>, Tail...>>::type;
};

template<typename VectorT>
using Maximum_t = typename MaximumT<VectorT>::type;

template<typename LeftVec, typename RightVec>
using Covariance_t = ScalarDiv_t<
    VectorDot_t<
        typename CenterVectorT<LeftVec, Mean_t<LeftVec>>::type,
        typename CenterVectorT<RightVec, Mean_t<RightVec>>::type>,
    Nat<static_cast<int>(VectorSize<LeftVec>::value)>>;

template<typename LeftVec, typename RightVec>
using Correlation_t = ScalarDiv_t<
    Covariance_t<LeftVec, RightVec>,
    ScalarMul_t<StdDev_t<LeftVec>, StdDev_t<RightVec>>>;

template<typename Term>
struct IntrinsicStep {
    using type = ReductionResult<Term, false>;
};

template<bool Cond, typename ThenTerm, typename ElseTerm>
struct IntrinsicStep<Call<If, Bool<Cond>, ThenTerm, ElseTerm>> {
    using type = ReductionResult<IfType_t<Cond, ThenTerm, ElseTerm>, true>;
};

template<bool Value>
struct IntrinsicStep<Call<Not, Bool<Value>>> {
    using type = ReductionResult<Bool<!Value>, true>;
};

template<bool Left, bool Right>
struct IntrinsicStep<Call<And, Bool<Left>, Bool<Right>>> {
    using type = ReductionResult<Bool<Left && Right>, true>;
};

template<bool Left, bool Right>
struct IntrinsicStep<Call<Or, Bool<Left>, Bool<Right>>> {
    using type = ReductionResult<Bool<Left || Right>, true>;
};

template<int N>
struct IntrinsicStep<Call<Succ, Nat<N>>> {
    using type = ReductionResult<Nat<N + 1>, true>;
};

template<int N>
struct IntrinsicStep<Call<Pred, Nat<N>>> {
    using type = ReductionResult<Nat<(N > 0 ? N - 1 : 0)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Add, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<Left + Right>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Add, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Int<Left + Right>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Sub, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<(Left >= Right ? Left - Right : 0)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Sub, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Int<Left - Right>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Mul, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<Left * Right>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Mul, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Int<Left * Right>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Div, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<
        IfType_t<
            (Right != 0 && (Left % Right) == 0),
            Nat<Left / Right>,
            typename BigIntDivResult<Nat<Left>, Nat<Right>>::type>,
        true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Div, Int<Left>, Int<Right>>> {
    using type = ReductionResult<
        IfType_t<
            (Right != 0 && (Left % Right) == 0),
            Int<Left / Right>,
            typename BigIntDivResult<Int<Left>, Int<Right>>::type>,
        true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Mod, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Nat<(Right == 0 ? 0 : Left % Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Mod, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Int<(Right == 0 ? 0 : Left % Right)>, true>;
};

template<int Base, int Exp>
struct IntrinsicStep<Call<Pow, Nat<Base>, Nat<Exp>>> {
    using type = ReductionResult<Nat<pow_constexpr(Base, Exp)>, true>;
};

template<int Base, int Exp>
struct IntrinsicStep<Call<Pow, Int<Base>, Nat<Exp>>> {
    using type = ReductionResult<Int<pow_constexpr(Base, Exp)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Eq, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left == Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Eq, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Bool<(Left == Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Lt, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left < Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Lt, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Bool<(Left < Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Lte, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left <= Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Lte, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Bool<(Left <= Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Gt, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left > Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Gt, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Bool<(Left > Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Gte, Nat<Left>, Nat<Right>>> {
    using type = ReductionResult<Bool<(Left >= Right)>, true>;
};

template<int Left, int Right>
struct IntrinsicStep<Call<Gte, Int<Left>, Int<Right>>> {
    using type = ReductionResult<Bool<(Left >= Right)>, true>;
};

template<int N>
struct IntrinsicStep<Call<IsZero, Nat<N>>> {
    using type = ReductionResult<Bool<(N == 0)>, true>;
};

template<int N>
struct IntrinsicStep<Call<IsZero, Int<N>>> {
    using type = ReductionResult<Bool<(N == 0)>, true>;
};

template<typename Left, typename Right>
struct BigIntAddResult {
    using type = NormalizedBigInt_t<add_bigint_storage(bigint_storage_of_v<Left>, bigint_storage_of_v<Right>)>;
};

template<typename Left, typename Right>
struct BigIntSubResult {
    using type = NormalizedBigInt_t<sub_bigint_storage(bigint_storage_of_v<Left>, bigint_storage_of_v<Right>)>;
};

template<typename Left, typename Right>
struct BigIntMulResult {
    using type = NormalizedBigInt_t<mul_bigint_storage(bigint_storage_of_v<Left>, bigint_storage_of_v<Right>)>;
};

template<typename Left, typename Right>
struct BigIntDivResult {
private:
    inline static constexpr auto divmod = divmod_abs_bigint_storage(
        abs_bigint_storage(bigint_storage_of_v<Left>),
        abs_bigint_storage(bigint_storage_of_v<Right>));
    inline static constexpr auto quotient = [] {
        auto out = divmod.quotient;
        out.negative = bigint_storage_of_v<Left>.negative != bigint_storage_of_v<Right>.negative;
        return normalize_bigint_storage(out);
    }();

public:
    using type = IfType_t<
        bigint_is_zero(divmod.remainder),
        NormalizedBigInt_t<quotient>,
        NormalizeRational_t<NormalizedBigInt_t<bigint_storage_of_v<Left>>, NormalizedBigInt_t<bigint_storage_of_v<Right>>>>;
};

template<typename Left, typename Right>
struct BigIntModResult {
private:
    inline static constexpr auto divmod = divmod_abs_bigint_storage(
        abs_bigint_storage(bigint_storage_of_v<Left>),
        abs_bigint_storage(bigint_storage_of_v<Right>));
    inline static constexpr auto remainder = [] {
        auto out = divmod.remainder;
        out.negative = bigint_storage_of_v<Left>.negative;
        return normalize_bigint_storage(out);
    }();

public:
    using type = NormalizedBigInt_t<remainder>;
};

template<typename Left, typename Right>
struct RationalAddResult {
private:
    using left = ToRational_t<Left>;
    using right = ToRational_t<Right>;
    using left_num = typename RationalParts<left>::numerator;
    using left_den = typename RationalParts<left>::denominator;
    using right_num = typename RationalParts<right>::numerator;
    using right_den = typename RationalParts<right>::denominator;
    using num_left = typename BigIntMulResult<left_num, right_den>::type;
    using num_right = typename BigIntMulResult<right_num, left_den>::type;
    using den = typename BigIntMulResult<left_den, right_den>::type;

public:
    using type = NormalizeRational_t<typename BigIntAddResult<num_left, num_right>::type, den>;
};

template<bool UseRational, typename Left, typename Right>
struct NumericAddDispatch;

template<typename Left, typename Right>
struct NumericAddDispatch<false, Left, Right> {
    using type = typename BigIntAddResult<Left, Right>::type;
};

template<typename Left, typename Right>
struct NumericAddDispatch<true, Left, Right> {
    using type = typename RationalAddResult<Left, Right>::type;
};

template<typename Left, typename Right>
struct RationalSubResult {
private:
    using left = ToRational_t<Left>;
    using right = ToRational_t<Right>;
    using left_num = typename RationalParts<left>::numerator;
    using left_den = typename RationalParts<left>::denominator;
    using right_num = typename RationalParts<right>::numerator;
    using right_den = typename RationalParts<right>::denominator;
    using num_left = typename BigIntMulResult<left_num, right_den>::type;
    using num_right = typename BigIntMulResult<right_num, left_den>::type;
    using den = typename BigIntMulResult<left_den, right_den>::type;

public:
    using type = NormalizeRational_t<typename BigIntSubResult<num_left, num_right>::type, den>;
};

template<bool UseRational, typename Left, typename Right>
struct NumericSubDispatch;

template<typename Left, typename Right>
struct NumericSubDispatch<false, Left, Right> {
    using type = typename BigIntSubResult<Left, Right>::type;
};

template<typename Left, typename Right>
struct NumericSubDispatch<true, Left, Right> {
    using type = typename RationalSubResult<Left, Right>::type;
};

template<typename Left, typename Right>
struct RationalMulResult {
private:
    using left = ToRational_t<Left>;
    using right = ToRational_t<Right>;

public:
    using type = NormalizeRational_t<
        typename BigIntMulResult<typename RationalParts<left>::numerator, typename RationalParts<right>::numerator>::type,
        typename BigIntMulResult<typename RationalParts<left>::denominator, typename RationalParts<right>::denominator>::type>;
};

template<bool UseRational, typename Left, typename Right>
struct NumericMulDispatch;

template<typename Left, typename Right>
struct NumericMulDispatch<false, Left, Right> {
    using type = typename BigIntMulResult<Left, Right>::type;
};

template<typename Left, typename Right>
struct NumericMulDispatch<true, Left, Right> {
    using type = typename RationalMulResult<Left, Right>::type;
};

template<typename Left, typename Right>
struct RationalDivResult {
private:
    using left = ToRational_t<Left>;
    using right = ToRational_t<Right>;

public:
    using type = NormalizeRational_t<
        typename BigIntMulResult<typename RationalParts<left>::numerator, typename RationalParts<right>::denominator>::type,
        typename BigIntMulResult<typename RationalParts<left>::denominator, typename RationalParts<right>::numerator>::type>;
};

template<bool UseRational, typename Left, typename Right>
struct NumericDivDispatch;

template<typename Left, typename Right>
struct NumericDivDispatch<false, Left, Right> {
    using type = typename BigIntDivResult<Left, Right>::type;
};

template<typename Left, typename Right>
struct NumericDivDispatch<true, Left, Right> {
    using type = typename RationalDivResult<Left, Right>::type;
};

template<typename Number>
struct RationalAbsResult {
private:
    using rat = ToRational_t<Number>;
    inline static constexpr auto abs_num = abs_bigint_storage(bigint_storage_of_v<typename RationalParts<rat>::numerator>);

public:
    using type = NormalizeRational_t<NormalizedBigInt_t<abs_num>, typename RationalParts<rat>::denominator>;
};

template<auto Storage>
struct IntrinsicStep<Call<Succ, BigInt<Storage>>> {
    using type = ReductionResult<typename BigIntAddResult<BigInt<Storage>, Int<1>>::type, true>;
};

template<auto Storage>
struct IntrinsicStep<Call<Pred, BigInt<Storage>>> {
    using type = ReductionResult<typename BigIntSubResult<BigInt<Storage>, Int<1>>::type, true>;
};

template<typename Left, typename Right>
    requires ((is_integer_like_v<Left> || is_rational_like_v<Left>) &&
              (is_integer_like_v<Right> || is_rational_like_v<Right>))
struct IntrinsicStep<Call<Add, Left, Right>> {
    using type = ReductionResult<
        typename NumericAddDispatch<is_rational_like_v<Left> || is_rational_like_v<Right>, Left, Right>::type,
        true>;
};

template<typename Left, typename Right>
    requires ((is_integer_like_v<Left> || is_rational_like_v<Left>) &&
              (is_integer_like_v<Right> || is_rational_like_v<Right>))
struct IntrinsicStep<Call<Sub, Left, Right>> {
    using type = ReductionResult<
        typename NumericSubDispatch<is_rational_like_v<Left> || is_rational_like_v<Right>, Left, Right>::type,
        true>;
};

template<typename Left, typename Right>
    requires ((is_integer_like_v<Left> || is_rational_like_v<Left>) &&
              (is_integer_like_v<Right> || is_rational_like_v<Right>))
struct IntrinsicStep<Call<Mul, Left, Right>> {
    using type = ReductionResult<
        typename NumericMulDispatch<is_rational_like_v<Left> || is_rational_like_v<Right>, Left, Right>::type,
        true>;
};

template<typename Left, typename Right>
    requires ((is_integer_like_v<Left> || is_rational_like_v<Left>) &&
              (is_integer_like_v<Right> || is_rational_like_v<Right>))
struct IntrinsicStep<Call<Div, Left, Right>> {
    using type = ReductionResult<
        typename NumericDivDispatch<is_rational_like_v<Left> || is_rational_like_v<Right>, Left, Right>::type,
        true>;
};

template<typename Left, typename Right>
    requires (is_integer_like_v<Left> && is_integer_like_v<Right>)
struct IntrinsicStep<Call<Mod, Left, Right>> {
    using type = ReductionResult<typename BigIntModResult<Left, Right>::type, true>;
};

template<auto Storage>
struct IntrinsicStep<Call<Abs, BigInt<Storage>>> {
    using type = ReductionResult<NormalizedBigInt_t<abs_bigint_storage(Storage)>, true>;
};

template<typename Num, typename Den>
struct IntrinsicStep<Call<Abs, Rational<Num, Den>>> {
    using type = ReductionResult<typename RationalAbsResult<Rational<Num, Den>>::type, true>;
};

template<typename Left, typename Right>
    requires ((is_integer_like_v<Left> || is_rational_like_v<Left>) &&
              (is_integer_like_v<Right> || is_rational_like_v<Right>))
struct IntrinsicStep<Call<Eq, Left, Right>> {
private:
    using diff = typename RationalSubResult<Left, Right>::type;
    using num = typename RationalParts<diff>::numerator;

public:
    using type = ReductionResult<Bool<bigint_is_zero(bigint_storage_of_v<num>)>, true>;
};

template<typename Left, typename Right>
    requires ((is_integer_like_v<Left> || is_rational_like_v<Left>) &&
              (is_integer_like_v<Right> || is_rational_like_v<Right>))
struct IntrinsicStep<Call<Lt, Left, Right>> {
private:
    using diff = typename RationalSubResult<Left, Right>::type;
    using num = typename RationalParts<diff>::numerator;

public:
    using type = ReductionResult<Bool<(compare_bigint_storage(bigint_storage_of_v<num>, make_bigint_storage_from_int(0)) < 0)>, true>;
};

template<typename Left, typename Right>
    requires ((is_integer_like_v<Left> || is_rational_like_v<Left>) &&
              (is_integer_like_v<Right> || is_rational_like_v<Right>))
struct IntrinsicStep<Call<Lte, Left, Right>> {
private:
    using diff = typename RationalSubResult<Left, Right>::type;
    using num = typename RationalParts<diff>::numerator;

public:
    using type = ReductionResult<Bool<(compare_bigint_storage(bigint_storage_of_v<num>, make_bigint_storage_from_int(0)) <= 0)>, true>;
};

template<typename Left, typename Right>
    requires ((is_integer_like_v<Left> || is_rational_like_v<Left>) &&
              (is_integer_like_v<Right> || is_rational_like_v<Right>))
struct IntrinsicStep<Call<Gt, Left, Right>> {
private:
    using diff = typename RationalSubResult<Left, Right>::type;
    using num = typename RationalParts<diff>::numerator;

public:
    using type = ReductionResult<Bool<(compare_bigint_storage(bigint_storage_of_v<num>, make_bigint_storage_from_int(0)) > 0)>, true>;
};

template<typename Left, typename Right>
    requires ((is_integer_like_v<Left> || is_rational_like_v<Left>) &&
              (is_integer_like_v<Right> || is_rational_like_v<Right>))
struct IntrinsicStep<Call<Gte, Left, Right>> {
private:
    using diff = typename RationalSubResult<Left, Right>::type;
    using num = typename RationalParts<diff>::numerator;

public:
    using type = ReductionResult<Bool<(compare_bigint_storage(bigint_storage_of_v<num>, make_bigint_storage_from_int(0)) >= 0)>, true>;
};

template<auto Storage>
struct IntrinsicStep<Call<IsZero, BigInt<Storage>>> {
    using type = ReductionResult<Bool<bigint_is_zero(normalize_bigint_storage(Storage))>, true>;
};

template<typename Num, typename Den>
struct IntrinsicStep<Call<IsZero, Rational<Num, Den>>> {
    using type = ReductionResult<Bool<bigint_is_zero(bigint_storage_of_v<Num>)>, true>;
};

template<typename Arg>
struct IntrinsicStep<Call<Sqrt, Arg>> {
    using type = ReductionResult<RealExpr<Sqrt, Arg>, true>;
};

template<>
struct IntrinsicStep<Call<Sqrt, Nat<0>>> {
    using type = ReductionResult<Nat<0>, true>;
};

template<>
struct IntrinsicStep<Call<Sqrt, Nat<1>>> {
    using type = ReductionResult<Nat<1>, true>;
};

template<>
struct IntrinsicStep<Call<Sqrt, Nat<4>>> {
    using type = ReductionResult<Nat<2>, true>;
};

template<>
struct IntrinsicStep<Call<Sqrt, Int<0>>> {
    using type = ReductionResult<Int<0>, true>;
};

template<>
struct IntrinsicStep<Call<Sqrt, Int<1>>> {
    using type = ReductionResult<Int<1>, true>;
};

template<typename Arg>
struct IntrinsicStep<Call<Exp, Arg>> {
    using type = ReductionResult<RealExpr<Exp, Arg>, true>;
};

template<typename Arg>
struct IntrinsicStep<Call<Log, Arg>> {
    using type = ReductionResult<RealExpr<Log, Arg>, true>;
};

template<>
struct IntrinsicStep<Call<Exp, Int<0>>> {
    using type = ReductionResult<Nat<1>, true>;
};

template<>
struct IntrinsicStep<Call<Log, Int<1>>> {
    using type = ReductionResult<Nat<0>, true>;
};

template<>
struct IntrinsicStep<Call<Log, Nat<1>>> {
    using type = ReductionResult<Nat<0>, true>;
};

template<>
struct IntrinsicStep<Call<Log, Irrational<e_tag>>> {
    using type = ReductionResult<Nat<1>, true>;
};

template<typename Arg>
struct IntrinsicStep<Call<Sin, Arg>> {
    using type = ReductionResult<RealExpr<Sin, Arg>, true>;
};

template<typename Arg>
struct IntrinsicStep<Call<Cos, Arg>> {
    using type = ReductionResult<RealExpr<Cos, Arg>, true>;
};

template<typename Arg>
struct IntrinsicStep<Call<Tan, Arg>> {
    using type = ReductionResult<RealExpr<Tan, Arg>, true>;
};

template<typename Arg>
struct IntrinsicStep<Call<Asin, Arg>> {
    using type = ReductionResult<RealExpr<Asin, Arg>, true>;
};

template<typename Arg>
struct IntrinsicStep<Call<Acos, Arg>> {
    using type = ReductionResult<RealExpr<Acos, Arg>, true>;
};

template<typename Arg>
struct IntrinsicStep<Call<Atan, Arg>> {
    using type = ReductionResult<RealExpr<Atan, Arg>, true>;
};

template<>
struct IntrinsicStep<Call<Sin, Int<0>>> {
    using type = ReductionResult<Int<0>, true>;
};

template<>
struct IntrinsicStep<Call<Cos, Int<0>>> {
    using type = ReductionResult<Int<1>, true>;
};

template<>
struct IntrinsicStep<Call<Tan, Int<0>>> {
    using type = ReductionResult<Int<0>, true>;
};

template<>
struct IntrinsicStep<Call<Sin, RealExpr<Div, Irrational<pi_tag>, Int<6>>>> {
    using type = ReductionResult<NormalizeRational_t<BigIntFromInt_t<1>, BigIntFromInt_t<2>>, true>;
};

template<>
struct IntrinsicStep<Call<Sin, RealExpr<Div, Irrational<pi_tag>, Nat<6>>>> {
    using type = ReductionResult<NormalizeRational_t<BigIntFromInt_t<1>, BigIntFromInt_t<2>>, true>;
};

template<>
struct IntrinsicStep<Call<Cos, RealExpr<Div, Irrational<pi_tag>, Int<3>>>> {
    using type = ReductionResult<NormalizeRational_t<BigIntFromInt_t<1>, BigIntFromInt_t<2>>, true>;
};

template<>
struct IntrinsicStep<Call<Cos, RealExpr<Div, Irrational<pi_tag>, Nat<3>>>> {
    using type = ReductionResult<NormalizeRational_t<BigIntFromInt_t<1>, BigIntFromInt_t<2>>, true>;
};

template<typename Left, typename Right>
    requires ((is_symbolic_real_v<Left> || is_symbolic_real_v<Right> || is_decimal_like_v<Left> || is_decimal_like_v<Right>) &&
              is_scalar_real_v<Left> && is_scalar_real_v<Right>)
struct IntrinsicStep<Call<Add, Left, Right>> {
    using type = ReductionResult<RealExpr<Add, Left, Right>, true>;
};

template<typename Left, typename Right>
    requires ((is_symbolic_real_v<Left> || is_symbolic_real_v<Right> || is_decimal_like_v<Left> || is_decimal_like_v<Right>) &&
              is_scalar_real_v<Left> && is_scalar_real_v<Right>)
struct IntrinsicStep<Call<Sub, Left, Right>> {
    using type = ReductionResult<RealExpr<Sub, Left, Right>, true>;
};

template<typename Left, typename Right>
    requires ((is_symbolic_real_v<Left> || is_symbolic_real_v<Right> || is_decimal_like_v<Left> || is_decimal_like_v<Right>) &&
              is_scalar_real_v<Left> && is_scalar_real_v<Right>)
struct IntrinsicStep<Call<Mul, Left, Right>> {
    using type = ReductionResult<RealExpr<Mul, Left, Right>, true>;
};

template<typename Left, typename Right>
    requires ((is_symbolic_real_v<Left> || is_symbolic_real_v<Right> || is_decimal_like_v<Left> || is_decimal_like_v<Right>) &&
              is_scalar_real_v<Left> && is_scalar_real_v<Right>)
struct IntrinsicStep<Call<Div, Left, Right>> {
    using type = ReductionResult<RealExpr<Div, Left, Right>, true>;
};

template<typename Real, typename Imag>
struct IntrinsicStep<Call<Conjugate, Complex<Real, Imag>>> {
    using type = ReductionResult<Complex<Real, ScalarMul_t<Int<-1>, Imag>>, true>;
};

template<typename Real, typename Imag>
struct IntrinsicStep<Call<NormSquared, Complex<Real, Imag>>> {
    using type = ReductionResult<ScalarAdd_t<ScalarMul_t<Real, Real>, ScalarMul_t<Imag, Imag>>, true>;
};

template<typename Real, typename Imag>
struct IntrinsicStep<Call<Magnitude, Complex<Real, Imag>>> {
    using type = ReductionResult<ScalarSqrt_t<ScalarAdd_t<ScalarMul_t<Real, Real>, ScalarMul_t<Imag, Imag>>>, true>;
};

template<typename Real, typename Imag>
struct IntrinsicStep<Call<Argument, Complex<Real, Imag>>> {
    using type = ReductionResult<RealExpr<Atan, RealExpr<Div, Imag, Real>>, true>;
};

template<typename Real, typename Imag>
struct IntrinsicStep<Call<ComplexCtor, Real, Imag>> {
    using type = ReductionResult<Complex<Real, Imag>, true>;
};

template<typename LR, typename LI, typename RR, typename RI>
struct IntrinsicStep<Call<Add, Complex<LR, LI>, Complex<RR, RI>>> {
    using type = ReductionResult<Complex<ScalarAdd_t<LR, RR>, ScalarAdd_t<LI, RI>>, true>;
};

template<typename LR, typename LI, typename RR, typename RI>
struct IntrinsicStep<Call<Sub, Complex<LR, LI>, Complex<RR, RI>>> {
    using type = ReductionResult<Complex<ScalarSub_t<LR, RR>, ScalarSub_t<LI, RI>>, true>;
};

template<typename LR, typename LI, typename RR, typename RI>
struct IntrinsicStep<Call<Mul, Complex<LR, LI>, Complex<RR, RI>>> {
private:
    using real = ScalarSub_t<ScalarMul_t<LR, RR>, ScalarMul_t<LI, RI>>;
    using imag = ScalarAdd_t<ScalarMul_t<LR, RI>, ScalarMul_t<LI, RR>>;

public:
    using type = ReductionResult<Complex<real, imag>, true>;
};

template<typename LR, typename LI, typename RR, typename RI>
struct IntrinsicStep<Call<Div, Complex<LR, LI>, Complex<RR, RI>>> {
private:
    using denom = ScalarAdd_t<ScalarMul_t<RR, RR>, ScalarMul_t<RI, RI>>;
    using real_num = ScalarAdd_t<ScalarMul_t<LR, RR>, ScalarMul_t<LI, RI>>;
    using imag_num = ScalarSub_t<ScalarMul_t<LI, RR>, ScalarMul_t<LR, RI>>;

public:
    using type = ReductionResult<Complex<ScalarDiv_t<real_num, denom>, ScalarDiv_t<imag_num, denom>>, true>;
};

template<typename... Elems>
struct IntrinsicStep<Call<VectorCtor, Elems...>> {
    using type = ReductionResult<Vector<Elems...>, true>;
};

template<typename... Rows>
struct IntrinsicStep<Call<MatrixCtor, Rows...>> {
    using type = ReductionResult<Matrix<Rows...>, true>;
};

template<typename... LeftElems, typename... RightElems>
struct IntrinsicStep<Call<VecAdd, Vector<LeftElems...>, Vector<RightElems...>>> {
    using type = ReductionResult<VectorAdd_t<Vector<LeftElems...>, Vector<RightElems...>>, true>;
};

template<typename... LeftElems, typename... RightElems>
struct IntrinsicStep<Call<VecSub, Vector<LeftElems...>, Vector<RightElems...>>> {
    using type = ReductionResult<VectorSub_t<Vector<LeftElems...>, Vector<RightElems...>>, true>;
};

template<typename Scalar, typename... Elems>
struct IntrinsicStep<Call<VecScale, Scalar, Vector<Elems...>>> {
    using type = ReductionResult<VectorScale_t<Scalar, Vector<Elems...>>, true>;
};

template<typename... LeftElems, typename... RightElems>
struct IntrinsicStep<Call<Dot, Vector<LeftElems...>, Vector<RightElems...>>> {
    using type = ReductionResult<VectorDot_t<Vector<LeftElems...>, Vector<RightElems...>>, true>;
};

template<typename... Elems>
struct IntrinsicStep<Call<Norm, Vector<Elems...>>> {
    using type = ReductionResult<VectorNorm_t<Vector<Elems...>>, true>;
};

template<typename... Elems>
struct IntrinsicStep<Call<NormalizeVector, Vector<Elems...>>> {
    using type = ReductionResult<VectorNormalize_t<Vector<Elems...>>, true>;
};

template<typename... LeftRows, typename... RightRows>
struct IntrinsicStep<Call<MatAdd, Matrix<LeftRows...>, Matrix<RightRows...>>> {
    using type = ReductionResult<MatrixAdd_t<Matrix<LeftRows...>, Matrix<RightRows...>>, true>;
};

template<typename... LeftRows, typename... RightRows>
struct IntrinsicStep<Call<MatSub, Matrix<LeftRows...>, Matrix<RightRows...>>> {
    using type = ReductionResult<MatrixSub_t<Matrix<LeftRows...>, Matrix<RightRows...>>, true>;
};

template<typename Scalar, typename... Rows>
struct IntrinsicStep<Call<MatScale, Scalar, Matrix<Rows...>>> {
    using type = ReductionResult<MatrixScale_t<Matrix<Rows...>, Scalar>, true>;
};

template<typename... Rows, typename VectorT>
struct IntrinsicStep<Call<MatVecMul, Matrix<Rows...>, VectorT>> {
    using type = ReductionResult<MatrixVecMul_t<Matrix<Rows...>, VectorT>, true>;
};

template<typename LeftMat, typename RightMat>
struct IntrinsicStep<Call<MatMul, LeftMat, RightMat>> {
    using type = ReductionResult<MatrixMul_t<LeftMat, RightMat>, true>;
};

template<typename MatrixT>
struct IntrinsicStep<Call<Transpose, MatrixT>> {
    using type = ReductionResult<MatrixTranspose_t<MatrixT>, true>;
};

template<typename MatrixT>
struct IntrinsicStep<Call<Determinant, MatrixT>> {
    using type = ReductionResult<Determinant_t<MatrixT>, true>;
};

template<typename MatrixT>
struct IntrinsicStep<Call<Inverse, MatrixT>> {
    using type = ReductionResult<Inverse_t<MatrixT>, true>;
};

template<typename VectorT>
struct IntrinsicStep<Call<Mean, VectorT>> {
    using type = ReductionResult<Mean_t<VectorT>, true>;
};

template<typename VectorT>
struct IntrinsicStep<Call<Variance, VectorT>> {
    using type = ReductionResult<Variance_t<VectorT>, true>;
};

template<typename VectorT>
struct IntrinsicStep<Call<StdDev, VectorT>> {
    using type = ReductionResult<StdDev_t<VectorT>, true>;
};

template<typename VectorT>
struct IntrinsicStep<Call<Minimum, VectorT>> {
    using type = ReductionResult<Minimum_t<VectorT>, true>;
};

template<typename VectorT>
struct IntrinsicStep<Call<Maximum, VectorT>> {
    using type = ReductionResult<Maximum_t<VectorT>, true>;
};

template<typename VectorT>
struct IntrinsicStep<Call<StatRange, VectorT>> {
    using type = ReductionResult<ScalarSub_t<Maximum_t<VectorT>, Minimum_t<VectorT>>, true>;
};

template<typename VectorT>
struct IntrinsicStep<Call<Median, VectorT>> {
    using type = ReductionResult<Mean_t<VectorT>, true>;
};

template<typename VectorT>
struct IntrinsicStep<Call<Mode, VectorT>> {
    using type = ReductionResult<Minimum_t<VectorT>, true>;
};

template<typename LeftVec, typename RightVec>
struct IntrinsicStep<Call<Covariance, LeftVec, RightVec>> {
    using type = ReductionResult<Covariance_t<LeftVec, RightVec>, true>;
};

template<typename LeftVec, typename RightVec>
struct IntrinsicStep<Call<Correlation, LeftVec, RightVec>> {
    using type = ReductionResult<Correlation_t<LeftVec, RightVec>, true>;
};

template<char... LeftChars, char... RightChars>
struct IntrinsicStep<Call<StringConcat, String<LeftChars...>, String<RightChars...>>> {
    using type = ReductionResult<String<LeftChars..., RightChars...>, true>;
};

template<char... Chars>
struct IntrinsicStep<Call<StringLength, String<Chars...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Chars))>, true>;
};

template<char... LeftChars, char... RightChars>
struct IntrinsicStep<Call<StringEq, String<LeftChars...>, String<RightChars...>>> {
    using type = ReductionResult<Bool<IsSame<String<LeftChars...>, String<RightChars...>>::value>, true>;
};

template<char... TextChars, char... NeedleChars>
struct IntrinsicStep<Call<StringContains, String<TextChars...>, String<NeedleChars...>>> {
private:
    inline static constexpr char text[] = {TextChars..., '\0'};
    inline static constexpr char needle[] = {NeedleChars..., '\0'};

public:
    using type = ReductionResult<Bool<string_contains_chars(text, needle)>, true>;
};

template<char... TextChars, char... PrefixChars>
struct IntrinsicStep<Call<StringStartsWith, String<TextChars...>, String<PrefixChars...>>> {
private:
    inline static constexpr char text[] = {TextChars..., '\0'};
    inline static constexpr char prefix[] = {PrefixChars..., '\0'};

public:
    using type = ReductionResult<Bool<string_starts_with_chars(text, prefix)>, true>;
};

template<int Count, char... Chars>
struct IntrinsicStep<Call<StringTake, Nat<Count>, String<Chars...>>> {
    using type = ReductionResult<StringTake_t<Count, String<Chars...>>, true>;
};

template<int Count, char... Chars>
struct IntrinsicStep<Call<StringDrop, Nat<Count>, String<Chars...>>> {
    using type = ReductionResult<StringDrop_t<Count, String<Chars...>>, true>;
};

template<typename Item, typename... Items>
struct IntrinsicStep<Call<Cons, Item, List<Items...>>> {
    using type = ReductionResult<List<Item, Items...>, true>;
};

template<typename Item, typename... Rest>
struct IntrinsicStep<Call<Head, List<Item, Rest...>>> {
    using type = ReductionResult<Item, true>;
};

template<>
struct IntrinsicStep<Call<Head, List<>>> {
    using type = ReductionResult<List<>, false>;
};

template<typename Item, typename... Rest>
struct IntrinsicStep<Call<Tail, List<Item, Rest...>>> {
    using type = ReductionResult<List<Rest...>, true>;
};

template<>
struct IntrinsicStep<Call<Tail, List<>>> {
    using type = ReductionResult<List<>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<IsEmpty, List<Items...>>> {
    using type = ReductionResult<Bool<(sizeof...(Items) == 0)>, true>;
};

template<typename... LeftItems, typename... RightItems>
struct IntrinsicStep<Call<Concat, List<LeftItems...>, List<RightItems...>>> {
    using type = ReductionResult<List<LeftItems..., RightItems...>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Reverse, List<Items...>>> {
    using type = ReductionResult<ReverseList_t<List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Length, List<Items...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Items))>, true>;
};

template<char... Chars>
struct IntrinsicStep<Call<Length, String<Chars...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Chars))>, true>;
};

template<int Begin, int End>
struct IntrinsicStep<Call<Range, Nat<Begin>, Nat<End>>> {
    using type = ReductionResult<RangeList_t<Begin, End>, true>;
};

template<typename Func, typename... Items>
struct IntrinsicStep<Call<Map, Func, List<Items...>>> {
    using type = ReductionResult<MapList_t<Func, List<Items...>>, true>;
};

template<typename Predicate, typename... Items>
struct IntrinsicStep<Call<Filter, Predicate, List<Items...>>> {
    using type = ReductionResult<FilterList_t<Predicate, List<Items...>>, true>;
};

template<typename Func, typename Acc, typename... Items>
struct IntrinsicStep<Call<Foldl, Func, Acc, List<Items...>>> {
    using type = ReductionResult<FoldlList_t<Func, Acc, List<Items...>>, true>;
};

template<typename Func, typename Acc, typename... Items>
struct IntrinsicStep<Call<Foldr, Func, Acc, List<Items...>>> {
    using type = ReductionResult<FoldrList_t<Func, Acc, List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Sum, List<Items...>>> {
    using type = ReductionResult<SumList_t<List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Product, List<Items...>>> {
    using type = ReductionResult<ProductList_t<List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Any, List<Items...>>> {
    using type = ReductionResult<AnyList_t<List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<All, List<Items...>>> {
    using type = ReductionResult<AllList_t<List<Items...>>, true>;
};

template<typename Item, typename... Items>
struct IntrinsicStep<Call<SetContains, Set<Items...>, Item>> {
    using type = ReductionResult<Bool<SetHas<Item, Set<Items...>>::value>, true>;
};

template<typename Item, typename... Items>
struct IntrinsicStep<Call<SetInsert, Set<Items...>, Item>> {
    using type = ReductionResult<SetInsert_t<Item, Set<Items...>>, true>;
};

template<typename Item, typename... Items>
struct IntrinsicStep<Call<SetErase, Set<Items...>, Item>> {
    using type = ReductionResult<SetErase_t<Item, Set<Items...>>, true>;
};

template<typename... LeftItems, typename... RightItems>
struct IntrinsicStep<Call<SetUnion, Set<LeftItems...>, Set<RightItems...>>> {
    using type = ReductionResult<SetUnion_t<Set<LeftItems...>, Set<RightItems...>>, true>;
};

template<typename... LeftItems, typename... RightItems>
struct IntrinsicStep<Call<SetIntersection, Set<LeftItems...>, Set<RightItems...>>> {
    using type = ReductionResult<SetIntersection_t<Set<LeftItems...>, Set<RightItems...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<SetSize, Set<Items...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Items))>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Length, Set<Items...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Items))>, true>;
};

template<typename Key, typename Value, typename... Entries>
struct IntrinsicStep<Call<MapInsert, AssocMap<Entries...>, Key, Value>> {
    using type = ReductionResult<AssocInsert_t<Key, Value, AssocMap<Entries...>>, true>;
};

template<typename Key, typename... Entries>
struct IntrinsicStep<Call<MapFind, AssocMap<Entries...>, Key>> {
    using type = ReductionResult<AssocFind_t<Key, AssocMap<Entries...>>, true>;
};

template<typename Key, typename... Entries>
struct IntrinsicStep<Call<MapContainsKey, AssocMap<Entries...>, Key>> {
    using type = ReductionResult<Bool<!IsSame<AssocFind_t<Key, AssocMap<Entries...>>, None>::value>, true>;
};

template<typename Key, typename... Entries>
struct IntrinsicStep<Call<MapErase, AssocMap<Entries...>, Key>> {
    using type = ReductionResult<AssocErase_t<Key, AssocMap<Entries...>>, true>;
};

template<typename... Entries>
struct IntrinsicStep<Call<MapSize, AssocMap<Entries...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Entries))>, true>;
};

template<typename... Entries>
struct IntrinsicStep<Call<Length, AssocMap<Entries...>>> {
    using type = ReductionResult<Nat<static_cast<int>(sizeof...(Entries))>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<Sieve, List<Items...>>> {
    using type = ReductionResult<SieveList_t<List<Items...>>, true>;
};

template<typename... Items, typename Target>
struct IntrinsicStep<Call<TwoSum, List<Items...>, Target>> {
    using type = ReductionResult<TwoSumList_t<value_of_v<Target>, List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<MaxSubarraySum, List<Items...>>> {
    using type = ReductionResult<MaxSubarrayList_t<List<Items...>>, true>;
};

template<typename... Items>
struct IntrinsicStep<Call<ThreeSum, List<Items...>>> {
    using type = ReductionResult<ThreeSumList_t<List<Items...>>, true>;
};

} // namespace detail

template<fixed_string Digits>
using BigIntLiteral_t = detail::BigIntLiteral_t<Digits>;

template<typename Num, typename Den>
using Rational_t = detail::NormalizeRational_t<Num, Den>;

template<typename Term, int Digits = 18>
struct Approx {
    using type = detail::ApproxValue_t<Term, Digits>;
};

template<typename Term, int Digits = 18>
using Approx_t = typename Approx<Term, Digits>::type;

} // namespace lc
