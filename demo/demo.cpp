#include <iostream>

#include "lambda.hpp"
#include "generated/lc/scripts.hpp"
#include "demo/demo.hpp"

namespace demo {

namespace {

using ScriptFactorialProgram = lc::ReadScript_t<lc::generated_scripts::factorial_source>;
using ScriptFactorialResult = lc::EvalScript_t<lc::generated_scripts::factorial_source>;
using ScriptFactorialType = lc::TypeCheckScript_t<lc::generated_scripts::factorial_source>;

using ScriptClosureProgram = lc::ReadScript_t<lc::generated_scripts::closure_source>;
using ScriptClosureResult = lc::EvalScript_t<lc::generated_scripts::closure_source>;

using ScriptListLengthResult = lc::EvalScript_t<lc::generated_scripts::list_length_source>;
using ScriptRationalResult = lc::EvalScript_t<lc::generated_scripts::rational_sum_source>;
using ScriptBigIntResult = lc::EvalScript_t<lc::generated_scripts::bigint_sum_source>;

using HugeA = lc::BigIntLiteral_t<"1234567890123456789012345678901234567890">;
using HugeB = lc::BigIntLiteral_t<"999999999999999999999999999999999999999">;
using HugeProduct = lc::Normalize_t<lc::Apply_t<lc::Mul, HugeA, HugeB>>;

template<typename Array>
void print_int_array(const Array& values) {
    std::cout << "[";
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << values[i];
    }
    std::cout << "]";
}

} // namespace

int run_demo() {
    constexpr auto primes = lc::to_array_v<lc::PrimesUpTo50>;
    constexpr auto two_sum = lc::to_array_v<lc::TwoSumExample>;
    constexpr auto three_sum = lc::to_matrix_v<lc::ThreeSumExample>;

    std::cout << "primes up to 50: ";
    print_int_array(primes);
    std::cout << "\n";

    std::cout << "count: " << lc::to_int_v<lc::PrimeCountUpTo50> << "\n";
    std::cout << "sum: " << lc::to_int_v<lc::PrimeSumUpTo50> << "\n";

    std::cout << "two-sum indices: ";
    print_int_array(two_sum);
    std::cout << "\n";

    std::cout << "maximum subarray sum: " << lc::to_int_v<lc::MaxSubarrayExample> << "\n";

    std::cout << "three-sum triplets: [";
    for (std::size_t i = 0; i < three_sum.size(); ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << "[" << three_sum[i][0] << ", " << three_sum[i][1] << ", " << three_sum[i][2] << "]";
    }
    std::cout << "]\n";

    std::cout << "script closure program: " << lc::pretty_string_view_v<ScriptClosureProgram> << "\n";
    std::cout << "script closure result: " << lc::to_int_v<ScriptClosureResult> << "\n";
    std::cout << "script factorial program: " << lc::pretty_string_view_v<ScriptFactorialProgram> << "\n";
    std::cout << "script factorial result: " << lc::to_int_v<ScriptFactorialResult> << "\n";
    std::cout << "script factorial type: " << lc::pretty_string_view_v<ScriptFactorialType> << "\n";
    std::cout << "script list length: " << lc::to_int_v<ScriptListLengthResult> << "\n";
    std::cout << "exact rational sum 1/3 + 1/6: " << lc::to_rational_string_view_v<ScriptRationalResult> << "\n";
    std::cout << "script bigint sum: " << lc::to_bigint_string_view_v<ScriptBigIntResult> << "\n";
    std::cout << "huge bigint product: " << lc::to_bigint_string_view_v<HugeProduct> << "\n";
    std::cout << "core lambda identity pretty: " << lc::pretty_string_view_v<lc::I> << "\n";

    return 0;
}

} // namespace demo
