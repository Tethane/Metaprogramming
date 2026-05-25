#include <iostream>

#include "lambda.hpp"

int main() {
    constexpr auto primes = lc::to_array_v<lc::PrimesUpTo50>;
    constexpr auto two_sum = lc::to_array_v<lc::TwoSumExample>;
    constexpr auto three_sum = lc::to_matrix_v<lc::ThreeSumExample>;

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
}
