/**
 * @file 1_c_style_array_basics.cpp
 * @brief Demonstrates C-style array declaration, initialization, and sizeof vs std::size
 * 
 * Key Concepts:
 * 1. C-style arrays are fixed-size arrays built directly into the language.
 * 2. Length must be a compile-time constant expression >= 1.
 * 3. Length can be omitted when all initializers are provided: int arr[]{ 1, 2, 3 };
 * 4. sizeof(arr) returns total bytes (length * sizeof(element)).
 * 5. std::size(arr) (C++17) returns element count for non-decayed arrays.
 * 6. C-style arrays do NOT support direct assignment; use std::copy or element-by-element assignment.
 */

#include <iostream>
#include <algorithm> // for std::copy

int main()
{
    // 1. Explicit length with aggregate initialization
    int a[5]{ 1, 2, 3, 4, 5 };

    // 2. Length deduction from initializers
    int primes[]{ 2, 3, 5, 7, 11, 13 };

    // 3. sizeof vs std::size
    std::cout << "sizeof(primes):    " << sizeof(primes) << " bytes\n";
    std::cout << "std::size(primes): " << std::size(primes) << " elements\n";

    // 4. C-style arrays do NOT support assignment
    int target[3]{ 0, 0, 0 };
    int source[3]{ 10, 20, 30 };
    // target = source; // COMPILE ERROR: array assignment is not supported!

    // Reassignment workaround: std::copy
    std::copy(std::begin(source), std::end(source), std::begin(target));
    std::cout << "target after std::copy: " << target[0] << ", " << target[1] << ", " << target[2] << '\n';

    return 0;
}
