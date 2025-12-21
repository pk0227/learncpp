/*
 * Demonstration: DANGEROUS - Returning string_view to Local Variables
 * 
 * THE PROBLEM:
 * - Function creates local std::string variables
 * - Function returns string_view to these locals
 * - Locals are destroyed when function ends
 * - Returned string_view is DANGLING REFERENCE
 * 
 * LIFETIME RULE:
 * Local variables die when function scope ends!
 * Returning a view to them = returning pointer to freed memory
 * 
 * NEVER DO THIS:
 * Don't return string_view to local std::string objects
 */

#include <iostream>
#include <string>
#include <string_view>

// DANGEROUS FUNCTION!
std::string_view getBoolName(bool b)
{
    // These are LOCAL variables (automatic storage duration)
    std::string t { "true" };   // Lives only in this function
    std::string f { "false" };  // Lives only in this function

    if (b)
        return t;  // Returns string_view viewing 't'
                   // BUT: 't' will be destroyed in a moment!

    return f;      // Returns string_view viewing 'f'
                   // BUT: 'f' will be destroyed in a moment!
                   
} // ☠️ BOTH 't' and 'f' DESTROYED HERE!
  // Any returned string_view now points to deallocated memory!
  // Caller receives DANGLING REFERENCE

int main()
{
    // UNDEFINED BEHAVIOR!
    // Both calls return string_view to destroyed local variables
    // Might crash, print garbage, or "work" by accident
    std::cout << getBoolName(true) << ' ' << getBoolName(false) << '\n';
    // ☠️ Accessing memory that was freed!
    
    // CORRECT APPROACH (see next example):
    // Return string_view to string LITERALS (permanent storage)
    // Or return std::string by value (owns the data)

    return 0;
}