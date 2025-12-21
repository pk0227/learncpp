/*
 * Demonstration: constexpr vs const - Understanding Compile-Time Constants
 * 
 * KEY DIFFERENCES:
 * constexpr: Must be evaluable at COMPILE-TIME
 *           - Requires constant expressions (literals, constexpr vars, constexpr functions)
 *           - More strict, safer for compile-time optimizations
 * 
 * const:    Can be initialized with RUNTIME values
 *           - More flexible, but not necessarily compile-time
 *           - Runtime const = "read-only variable"
 * 
 * Rule of Thumb:
 * - Use constexpr when value must be known at compile-time
 * - Use const when value is computed at runtime but shouldn't change
 */

#include <iostream>

// NON-constexpr function: Return value is computed at runtime
// Even though it always returns 5, compiler treats it as runtime calculation
int five()
{
    return 5;
}

int main()
{
    // VALID constexpr declarations (all evaluated at compile-time):
    
    // Literal values are constant expressions
    constexpr double gravity { 9.8 };  // ✓ Literal 9.8 known at compile-time
    
    // Arithmetic with literals is a constant expression
    constexpr int sum { 4 + 5 };       // ✓ 4 + 5 evaluated at compile-time = 9
    
    // constexpr variables are constant expressions
    constexpr int something { sum };   // ✓ sum is constexpr, so value known at compile-time
    
    // ============================================================================

    // Runtime input: Value not known until program runs
    std::cout << "Enter your age: ";
    int age{};
    std::cin >> age;  // age receives value at RUNTIME

    // INVALID constexpr declarations (would be compile errors):
    
    // ✗ age is a runtime value, not a compile-time constant
    //constexpr int myAge { age };      // ERROR: age not evaluable at compile-time
    
    // ✗ five() is not a constexpr function, so return value is runtime
    //constexpr int f { five() };       // ERROR: five() not constexpr

    // VALID const declarations (initialized at runtime):
    
    // ✓ const allows runtime initialization
    const int myAge { age };       // OK: const can be initialized with runtime value
    
    // ✓ const works with non-constexpr functions
    const int f { five() };        // OK: const doesn't require compile-time evaluation

    return 0;
}