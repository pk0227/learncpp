/*
 * Demonstration: SAFE - Returning string_view to String Literals
 * 
 * THE SOLUTION:
 * - String literals have STATIC storage duration
 * - They live for the entire program lifetime
 * - Safe to return string_view to them!
 * 
 * STRING LITERAL LIFETIME:
 * "text" lives in read-only program memory
 * Exists from program start to program end
 * Never deallocated
 * 
 * RULE:
 * It's safe to return string_view to:
 * - String literals ("text")
 * - Static/global strings
 * - Any data guaranteed to outlive the caller
 */

#include <iostream>
#include <string_view>

// SAFE FUNCTION!
std::string_view getBoolName(bool b)
{
    if (b)
        // "true" is a string LITERAL (permanent storage)
        // Lives forever in program memory
        return "true";   // ✓ Safe: returns view to permanent literal

    // "false" is also a string LITERAL
    return "false";      // ✓ Safe: returns view to permanent literal
    
} // Literals are NOT destroyed - they're permanent!
  // Returned string_view remains valid forever

int main()
{
    // PERFECTLY SAFE!
    // Both string_views reference permanent string literals
    // No dangling references, no undefined behavior
    std::cout << getBoolName(true) << ' ' << getBoolName(false) << '\n';
    // ✓ Prints: true false
    
    // KEY LESSON:
    // Returning string_view is safe when viewing:
    // 1. String literals ("text")
    // 2. Static variables
    // 3. Global variables
    // 4. Any data with lifetime > function duration
    // 
    // NEVER return string_view to:
    // 1. Local variables
    // 2. Temporary objects
    // 3. Data that will be destroyed

    return 0;
}