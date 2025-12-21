/*
 * Demonstration: Getting String Length in C++
 * 
 * TWO APPROACHES:
 * 1. .length() method - Returns unsigned size_t
 * 2. std::ssize() function (C++20) - Returns signed std::ptrdiff_t
 * 
 * WHY SIGNED SIZE MATTERS:
 * - Unsigned types can cause subtle bugs in arithmetic (e.g., 0 - 1 wraps to huge number)
 * - Signed types safer for calculations involving subtraction
 * - std::ssize() provides safer alternative to .length()
 * 
 * CASTING GUIDELINES:
 * - Always static_cast when converting between signed/unsigned
 * - Avoids compiler warnings about implicit conversions
 * - Makes intent explicit and code safer
 */

#include <iostream>
#include <string>

int main()
{
    std::string name{ "Rama" };
    
    // METHOD 1: .length() - Returns size_t (unsigned integral type)
    // ISSUE: size_t is unsigned (usually size_t = unsigned long long)
    // - Can't represent negative numbers
    // - Arithmetic with int can cause issues
    // SOLUTION: Cast to int for safe storage
    int length { static_cast<int>(name.length()) };
    
    // METHOD 2: std::ssize() - C++20 function returns signed size
    // Returns: std::ptrdiff_t (signed integral type, usually long long)
    // BENEFITS:
    // - Signed type is safer for arithmetic operations
    // - Better for loop counters that might go negative
    // - More intuitive for programmers used to int
    // 
    // NOTE: std::ptrdiff_t may be larger than int (e.g., 64-bit vs 32-bit)
    // If storing in int, still need static_cast
    int len { static_cast<int>(std::ssize(name)) };

    // DIRECT OUTPUT (without storing in variable):
    // These work without casting because cout handles both types
    //std::cout << name << " has " << name.length() << " characters\n";
    //std::cout << name << " has " << std::ssize(name) << " characters\n";
    
    // OUTPUT WITH CASTED VALUES:
    std::cout << name << " has " << length << " characters\n";
    std::cout << name << " has " << len << " characters\n";
    // Both print: "Rama has 4 characters"

    return 0;
}