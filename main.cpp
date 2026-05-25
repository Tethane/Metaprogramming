#include <iostream>

#include "lambda.hpp"

int main() {
    constexpr auto primes = lc::to_array_v<lc::PrimesUpTo50>;
    constexpr auto two_sum = lc::to_array_v<lc::TwoSumExample>;
    constexpr auto three_sum = lc::to_matrix_v<lc::ThreeSumExample>;
    constexpr auto union_set = lc::to_array_v<lc::UnionSetExample>;

    std::cout << "primes up to 50: [";
    for (std::size_t i = 0; i < primes.size(); ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << primes[i];
    }
    std::cout << "]\n";

    std::cout << "count: " << lc::to_int_v<lc::PrimeCountUpTo50> << "\n";
    std::cout << "sum: " << lc::to_int_v<lc::PrimeSumUpTo50> << "\n";

    std::cout << "two-sum indices for [2, 7, 11, 15], target 9: ["
              << two_sum[0] << ", " << two_sum[1] << "]\n";

    std::cout << "maximum subarray sum for [-2, 1, -3, 4, -1, 2, 1, -5, 4]: "
              << lc::to_int_v<lc::MaxSubarrayExample> << "\n";

    std::cout << "three-sum triplets for [-4, -1, -1, 0, 1, 2]: [";
    for (std::size_t i = 0; i < three_sum.size(); ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << "[" << three_sum[i][0] << ", " << three_sum[i][1] << ", " << three_sum[i][2] << "]";
    }
    std::cout << "]\n";

    std::cout << "string concat example: " << lc::to_string_view_v<lc::LambdaCalculusString> << "\n";
    std::cout << "string prefix (take 6): " << lc::to_string_view_v<lc::LambdaPrefix> << "\n";
    std::cout << "string contains \"calc\": " << (lc::to_bool_v<lc::StringContainsCalc> ? "true" : "false") << "\n";

    std::cout << "set union of {1, 3, 5} and {3, 4, 5}: [";
    for (std::size_t i = 0; i < union_set.size(); ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << union_set[i];
    }
    std::cout << "]\n";
    std::cout << "set contains 4: " << (lc::to_bool_v<lc::SetContainsFourExample> ? "true" : "false") << "\n";

    std::cout << "map lookup \"answer\": " << lc::to_int_v<lc::MapLookupAnswer> << "\n";
    std::cout << "map contains \"lucky\": " << (lc::to_bool_v<lc::MapContainsLucky> ? "true" : "false") << "\n";

    std::cout << "lisp closure program: " << lc::pretty_string_view_v<lc::LispClosureProgram> << "\n";
    std::cout << "lisp closure result: " << lc::to_int_v<lc::LispClosureResult> << "\n";
    std::cout << "lisp closure type: " << lc::pretty_string_view_v<lc::LispClosureResultType> << "\n";
    std::cout << "lisp string result: " << lc::to_string_view_v<lc::LispStringResult> << "\n";
    std::cout << "core lambda identity pretty: " << lc::pretty_string_view_v<lc::I> << "\n";
    std::cout << "reader closure program: " << lc::pretty_string_view_v<lc::ReaderClosureProgram> << "\n";
    std::cout << "reader closure result: " << lc::to_int_v<lc::ReaderClosureResult> << "\n";
    std::cout << "reader quoted list: " << lc::pretty_string_view_v<lc::ReaderQuotedListResult> << "\n";
    std::cout << "reader untyped identity result: " << lc::to_int_v<lc::ReaderUntypedIdentityResult> << "\n";
    std::cout << "reader comment program result: " << lc::to_string_view_v<lc::ReaderCommentResult> << "\n";
    std::cout << "reader escaped string pretty: " << lc::pretty_string_view_v<lc::ReaderEscapedStringResult> << "\n";
    std::cout << "reader factorial via define/cond: " << lc::to_int_v<lc::ReaderDefinedFactorialResult> << "\n";
    std::cout << "reader list inference result: " << lc::to_int_v<lc::ReaderListInferenceResult> << "\n";
    std::cout << "reader list expansion: " << lc::pretty_string_view_v<lc::ReaderExpandList> << "\n";
    std::cout << "reader cond expansion: " << lc::pretty_string_view_v<lc::ReaderExpandCond> << "\n";
    std::cout << "reader recursive value define error: " << lc::pretty_string_view_v<lc::ReaderRecursiveValueDefineError> << "\n";
    std::cout << "reader parse error pretty: " << lc::pretty_string_view_v<lc::ReadSource_t<"(+ 1 2">> << "\n";
}
