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
using ScriptMathSymbolicProgram = lc::ReadScript_t<lc::generated_scripts::math_symbolic_source>;
using ScriptMathSymbolicResult = lc::EvalScript_t<lc::generated_scripts::math_symbolic_source>;
using ScriptMathApproxResult = lc::ReadScriptApprox_t<lc::generated_scripts::math_approx_source, 12>;
using ScriptVectorMathResult = lc::EvalScript_t<lc::generated_scripts::vector_math_source>;
using ScriptMatrixMathResult = lc::EvalScript_t<lc::generated_scripts::matrix_math_source>;
using ScriptComplexMathResult = lc::EvalScript_t<lc::generated_scripts::complex_math_source>;
using ScriptStatsMathResult = lc::EvalScript_t<lc::generated_scripts::stats_math_source>;
using HaskellFactorialResult = lc::EvalHaskellScript_t<lc::generated_haskell_scripts::factorial_source>;
using HaskellFactorialType = lc::TypeCheckHaskellScript_t<lc::generated_haskell_scripts::factorial_source>;
using HaskellMaybeResult = lc::EvalHaskellScript_t<lc::generated_haskell_scripts::maybe_int_source>;
using HaskellMaybeType = lc::TypeCheckHaskellScript_t<lc::generated_haskell_scripts::maybe_int_source>;
using HaskellStreamResult = lc::EvalHaskellScript_t<lc::generated_haskell_scripts::stream_take_source>;
using HaskellStreamType = lc::TypeCheckHaskellScript_t<lc::generated_haskell_scripts::stream_take_source>;
using HaskellIoRun = lc::RunHaskellScript_t<lc::generated_haskell_scripts::io_demo_source, 0>;
using HaskellIoType = lc::TypeCheckHaskellScript_t<lc::generated_haskell_scripts::io_demo_source>;
using HaskellWhereResult = lc::EvalHaskellSource_t<"main :: Int; main = let { y = 20 + 22 } in y;">;
using HaskellLambdaResult = lc::EvalHaskellSource_t<"main :: Int; main = (\\x -> x + 1) 41;">;
using DotStats = lc::NormalizeWithStats_t<lc::Apply_t<lc::Dot, lc::VectorExampleA, lc::VectorExampleB>>;
using MatrixStats = lc::NormalizeWithStats_t<lc::Apply_t<lc::MatMul, lc::MatrixExampleA, lc::MatrixExampleB>>;

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
    constexpr auto haskell_stream = lc::to_array_v<HaskellStreamResult>;

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
    std::cout << "symbolic math program: " << lc::pretty_string_view_v<ScriptMathSymbolicProgram> << "\n";
    std::cout << "sin(pi / 6): " << lc::to_rational_string_view_v<ScriptMathSymbolicResult> << "\n";
    std::cout << "sqrt(2) symbolic: " << lc::pretty_string_view_v<lc::SymbolicSqrtTwo> << "\n";
    std::cout << "sqrt(2) approx: " << lc::to_decimal_string_view_v<lc::ApproxSqrtTwo> << "\n";
    std::cout << "exp(1) approx: " << lc::to_decimal_string_view_v<ScriptMathApproxResult> << "\n";
    std::cout << "vector dot product: " << lc::to_int_v<ScriptVectorMathResult> << "\n";
    std::cout << "matrix product: " << lc::pretty_string_view_v<ScriptMatrixMathResult> << "\n";
    std::cout << "matrix determinant: " << lc::to_int_v<lc::MatrixDeterminantExample> << "\n";
    std::cout << "matrix inverse: " << lc::pretty_string_view_v<lc::MatrixInverseExample> << "\n";
    std::cout << "complex sum: " << lc::pretty_string_view_v<ScriptComplexMathResult> << "\n";
    std::cout << "complex magnitude approx: " << lc::to_decimal_string_view_v<lc::ApproxComplexMagnitudeExample> << "\n";
    std::cout << "vector mean: " << lc::to_rational_string_view_v<ScriptStatsMathResult> << "\n";
    std::cout << "dot stats: reductions=" << lc::reduction_count_v<DotStats>
              << " nodes=" << lc::node_count_v<DotStats> << "\n";
    std::cout << "matrix stats: reductions=" << lc::reduction_count_v<MatrixStats>
              << " nodes=" << lc::node_count_v<MatrixStats> << "\n";
    std::cout << "core lambda identity pretty: " << lc::pretty_string_view_v<lc::I> << "\n";
    std::cout << "haskell factorial: " << lc::to_int_v<HaskellFactorialResult>
              << " : " << lc::pretty_string_view_v<HaskellFactorialType> << "\n";
    std::cout << "haskell maybe pattern match: " << lc::to_int_v<HaskellMaybeResult>
              << " : " << lc::pretty_string_view_v<HaskellMaybeType> << "\n";
    std::cout << "haskell lazy stream prefix: ";
    print_int_array(haskell_stream);
    std::cout << " : " << lc::pretty_string_view_v<HaskellStreamType> << "\n";
    std::cout << "haskell let result: " << lc::to_int_v<HaskellWhereResult> << "\n";
    std::cout << "haskell lambda result: " << lc::to_int_v<HaskellLambdaResult> << "\n";
    std::cout << "haskell io type: " << lc::pretty_string_view_v<HaskellIoType> << "\n";
    std::cout << "haskell io output: " << lc::to_output_string_view_v<HaskellIoRun>;
    std::cout << "haskell io value: " << lc::to_string_view_v<lc::to_effect_value_t<HaskellIoRun>> << "\n";
    std::cout << "haskell io next seed: " << lc::to_seed_v<HaskellIoRun> << "\n";

    return 0;
}

} // namespace demo
