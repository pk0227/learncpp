/*
 * Demonstration: DANGEROUS - string_view Viewing Temporary from Suffix
 * 
 * THE PROBLEM:
 * - "text"s creates temporary std::string (using literal suffix)
 * - Temporary destroyed at end of statement
 * - string_view left viewing destroyed temporary
 * 
 * STRING LITERAL SUFFIXES:
 * - "text"   = C-style string (const char*) - permanent, lives forever
 * - "text"s  = std::string (user-defined literal) - TEMPORARY object!
 * 
 * RULE:
 * Don't use "text"s suffix when initializing string_view
 * Use plain "text" instead (C-style string literal)
 */

#include <iostream>
#include <string>
#include <string_view>

int main()
{
    using namespace std::string_literals;  // Enables "text"s suffix
    
    // DANGEROUS CODE:
    // "Rama"s creates temporary std::string object (not a string literal!)
    // Temporary exists only for this statement
    // After semicolon, temporary destroyed
    // name left viewing deallocated memory
    std::string_view name { "Rama"s };
    
    // UNDEFINED BEHAVIOR!
    // The temporary std::string created by "Rama"s is already gone
    std::cout << name << '\n';  // ☠️ DANGER: Dangling reference
    
    // CORRECT APPROACH:
    // Option 1: Use plain string literal (no 's' suffix)
    // std::string_view name { "Rama" };  // ✓ Safe - views permanent literal
    
    // Option 2: Use std::string to own the data
    // std::string name { "Rama"s };  // ✓ Safe - owns a copy
    
    // KEY INSIGHT:
    // String literals ("text") live forever in program memory
    // String objects ("text"s) are temporary and will be destroyed
    // string_view should only view long-lived data!

    return 0;
}