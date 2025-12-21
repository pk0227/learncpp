/*
 * Demonstration: std::string_view - Avoiding Unnecessary Copies (C++17)
 * 
 * THE PROBLEM:
 * - std::string makes copies when passed by value
 * - Copying strings is EXPENSIVE (dynamic memory allocation)
 * - Most functions only need READ access, not ownership
 * 
 * THE SOLUTION: std::string_view
 * - Lightweight "view" into existing string data
 * - No copying, no allocation, just a pointer + length
 * - Read-only access (cannot modify the underlying string)
 * - Works with C-style strings, std::string, other string_views
 * 
 * PERFORMANCE:
 * - std::string: Allocates memory, copies characters (slow)
 * - std::string_view: Just stores pointer (fast, ~16 bytes typically)
 * 
 * USE CASE:
 * Perfect for function parameters that only need to read string data
 */

#include <iostream>
#include <string>
#include <string_view> // C++17

// OPTIMIZED: Uses string_view - no copy made
// str provides read-only access to whatever argument is passed in
void printSV(std::string_view str)
{
    std::cout << str << '\n';
    // str is just a "view" - a lightweight reference to the actual string
    // No memory allocation, no copying!
}

// INEFFICIENT: Takes string by value - makes a copy
void printString(std::string str)
{
    std::cout << str << '\n';
    // str is a COPY of the argument
    // Memory allocated, characters copied (expensive!)
}

int main()
{
    // INEFFICIENT APPROACH:
    // Copy 1: "Hello, world!" → s (dynamic allocation + copy)
    std::string s{ "Hello, world!" };
    
    // Copy 2: s → str parameter in printString (another allocation + copy)
    printString(s);
    
    // TOTAL: 2 copies of "Hello, world!" created
    // Memory allocated twice, characters copied twice
    // Just to print a string once! Wasteful!

    // -----------------------------------------------------------------------------------
    
    // EFFICIENT APPROACH:
    // No copy: string_view just references the literal
    std::string_view sv{ "Hello, world!" };
    
    // No copy: sv passed to printSV, which creates another view
    printSV(sv);
    
    // TOTAL: 0 copies made!
    // Original string literal is referenced twice
    // Much faster, no dynamic allocation
    
    // KEY INSIGHT:
    // std::string_view provides read-only access to an existing string
    // (C-style string, std::string, or another std::string_view)
    // WITHOUT making a copy
    // 
    // Read-only means:
    // - We can access and use the value
    // - We CANNOT modify it
    // - Perfect for functions that just need to read string data
    
    return 0;
}