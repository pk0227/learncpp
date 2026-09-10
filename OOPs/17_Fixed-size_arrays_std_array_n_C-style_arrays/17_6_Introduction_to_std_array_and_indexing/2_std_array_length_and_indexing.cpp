/**
 * @file 2_std_array_length_and_indexing.cpp
 * @brief Demonstrates std::array length retrieval and indexing methods
 * 
 * Key Concepts:
 * 1. arr.size() & std::size(arr): return unsigned size_type (std::size_t).
 * 2. std::ssize(arr) (C++20): returns signed integral type (std::ptrdiff_t).
 * 3. operator[]: fast, zero overhead, NO bounds checking (UB on invalid index).
 * 4. arr.at(i): runtime bounds checking (throws std::out_of_range).
 * 5. std::get<Index>(arr): compile-time bounds checking for constexpr indices.
 */

#include <iostream>
#include <array>
#include <stdexcept>

int main()
{
    constexpr std::array primes{ 2, 3, 5, 7, 11 };

    // 1. Length retrieval
    std::cout << "arr.size():  " << primes.size() << " (unsigned)\n";
    std::cout << "std::size:   " << std::size(primes) << " (unsigned)\n";
    std::cout << "std::ssize:  " << std::ssize(primes) << " (signed)\n";

    // std::size is constexpr, can initialize constexpr variables:
    constexpr int len{ std::size(primes) };
    static_assert(len == 5);

    // 2. Indexing with operator[] (no bounds checking)
    std::cout << "primes[2]: " << primes[2] << '\n';

    // 3. Indexing with at() (runtime bounds checking)
    try
    {
        std::cout << "primes.at(4): " << primes.at(4) << '\n';
        // primes.at(10); // Throws std::out_of_range
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "Caught exception: " << e.what() << '\n';
    }

    // 4. Indexing with std::get<Index>() (compile-time bounds checking)
    // std::get requires index to be a constant expression.
    // std::get<10>(primes); // COMPILE ERROR: index out of bounds!
    std::cout << "std::get<3>(primes): " << std::get<3>(primes) << '\n';

    return 0;
}
