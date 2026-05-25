#include <iostream>

#include "lambda.hpp"

int main() {
    constexpr auto primes = lc::to_array_v<lc::PrimesUpTo50>;

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
}
