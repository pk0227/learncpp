/**
 * Demonstration of narrowing conversion issues between signed and unsigned types
 * 
 * Key Concepts:
 * 1. List initialization {} disallows narrowing conversions for non-constexpr values
 * 2. Copy initialization (assignment/function calls) allows narrowing with warnings
 * 3. constexpr variables can be safely converted if the value fits in the target type
 * 
 * Compile with -Werror flag to treat warnings as errors
 */

#include <iostream>

// Function accepting unsigned int parameter
void foo(unsigned int)
{
    // Function body intentionally empty for demonstration
}

int main()
{
    // Non-constexpr signed integer
    int i{5};
    
    // List initialization with narrowing conversion
    // ISSUE: Conversion from signed int to unsigned int is considered narrowing
    // This will fail with -Werror flag because i is non-constexpr
    // If i were "constexpr int i{5};", it would compile successfully
    unsigned int ui{i};     

    // Copy initialization (function argument passing)
    // This allows narrowing conversion but may generate a compiler warning
    // The warning can be treated as an error with -Werror flag
    foo(i);                 

    std::cout << "Unsigned in : " << ui << "\n";

    return 0;
}