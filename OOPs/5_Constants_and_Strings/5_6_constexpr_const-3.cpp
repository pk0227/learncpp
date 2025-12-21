/*
 * Demonstration: constexpr vs Non-constexpr Functions
 * 
 * COMPARISON:
 * Regular function:      Always executes at runtime
 * constexpr function:    Can execute at compile-time (if args are constexpr)
 *                        Can also execute at runtime (if args are not constexpr)
 * 
 * USAGE RULES:
 * 1. Non-constexpr function result → Cannot initialize constexpr variable
 * 2. constexpr function with constexpr args → Can initialize constexpr variable
 * 3. constexpr function with runtime args → Executes at runtime
 */

#include <iostream>

// NON-constexpr function: Always evaluates at runtime
int max(int x, int y)
{
    if (x > y)
        return x;
    else
        return y;
}

// constexpr function: Can evaluate at compile-time or runtime
constexpr int cmax(int x, int y)
{
    // Same logic as max(), but can be compile-time evaluated
    if (x > y)
        return x;
    else
        return y;
}

int main()
{
    // ========== Using NON-constexpr function max() ==========
    
    // Regular variable: OK (no restrictions)
    int m1 { max(5, 6) };  // ✓ max() evaluates at runtime, result stored in m1
    
    // const variable: OK (const allows runtime initialization)
    const int m2 { max(5, 6) };  // ✓ Runtime evaluation, then marked read-only
    
    // constexpr variable: ERROR!
    // max() is not constexpr, so its return value is not a compile-time constant
    //constexpr int m3 { max(5, 6) };  // ✗ Compile error: max() is not a constant expression

    // ========== Using constexpr function cmax() ==========
    
    // Regular variable: OK
    // Compiler MAY optimize to compile-time, or evaluate at runtime
    int m4 { cmax(5, 6) };  // ✓ Flexible: compile-time OR runtime
    
    // const variable: OK
    // Compiler MAY optimize to compile-time, or evaluate at runtime
    const int m5 { cmax(5, 6) };  // ✓ Flexible: compile-time OR runtime
    
    // constexpr variable: OK and GUARANTEED compile-time evaluation
    // constexpr context FORCES compile-time evaluation
    constexpr int m6 { cmax(5, 6) };  // ✓ MUST evaluate at compile-time (value = 6)
    // The compiled code will have '6' directly embedded, no function call!

    return 0;
}