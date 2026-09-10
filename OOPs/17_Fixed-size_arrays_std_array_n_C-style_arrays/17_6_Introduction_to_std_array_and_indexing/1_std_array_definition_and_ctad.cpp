/**
 * @file 1_std_array_definition_and_ctad.cpp
 * @brief Demonstrates std::array definition, aggregate initialization, CTAD, and std::to_array
 * 
 * Key Concepts:
 * 1. std::array<T, N> requires element type T and a compile-time constant length N.
 * 2. Aggregate initialization: value initialization ({}) zeroes all elements.
 * 3. Class Template Argument Deduction (CTAD) since C++17 deduces both type and size.
 * 4. std::to_array (C++20) allows deducing size while explicitly specifying type (or deducing both).
 */

#include <iostream>
#include <array>

int main()
{
    // 1. Defining std::array with explicit template arguments
    constexpr int len{ 5 };
    std::array<int, len> a{}; // Value-initialized: all 5 elements are 0
    std::cout << "a[0]: " << a[0] << '\n';

    // 2. Aggregate initialization
    std::array<int, 5> b{ 1, 2, 3, 4, 5 }; // Fully initialized
    std::array<int, 5> c{ 1, 2 };          // Partial: c[0]=1, c[1]=2, remaining value-initialized (0)
    std::cout << "c[1]: " << c[1] << ", c[2]: " << c[2] << '\n';

    // 3. Class Template Argument Deduction (CTAD) - C++17
    // Deduces both type (int) and length (5)
    constexpr std::array primes{ 2, 3, 5, 7, 11 };
    std::cout << "primes size: " << primes.size() << ", first prime: " << primes[0] << '\n';

    // 4. std::to_array - C++20
    // Allows specifying element type while deducing array length
    constexpr auto shorts = std::to_array<short>({ 10, 20, 30 });
    static_assert(sizeof(shorts[0]) == sizeof(short));
    std::cout << "shorts size: " << shorts.size() << ", value: " << shorts[0] << '\n';

    return 0;
}
