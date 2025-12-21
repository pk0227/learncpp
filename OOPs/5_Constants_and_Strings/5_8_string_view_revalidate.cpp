/*
 * Demonstration: string_view Invalidation and Revalidation
 * 
 * CRITICAL ISSUE:
 * - string_view is just a pointer to string data
 * - If the UNDERLYING STRING is modified, the view may become INVALID
 * - Not because view changed, but because data it points to changed!
 * 
 * INVALIDATION SCENARIOS:
 * 1. String is modified (assignment, resize, etc.)
 * 2. String is reallocated (internal buffer moved)
 * 3. String is destroyed
 * 
 * SOLUTION:
 * After modifying the underlying string, REVALIDATE the view
 * (Create a new view to the modified string)
 */

#include <iostream>
#include <string>
#include <string_view>

int main()
{
    // Create std::string
    std::string s { "Hello, world!" };
    
    // Create string_view viewing s
    // sv stores pointer to s's internal buffer + length
    std::string_view sv { s };
    
    // MODIFY THE UNDERLYING STRING!
    // This assignment might:
    // 1. Reuse existing buffer (if new string fits)
    // 2. Allocate NEW buffer and free old one (if larger)
    s = "Hello, universe!";  // ⚠️ s's internal buffer may have moved!
    
    // PROBLEM: sv still points to the OLD buffer location
    // If buffer was reallocated, OLD buffer is now FREED memory
    // sv is now INVALID (dangling pointer)
    std::cout << sv << '\n';  // ⚠️ UNDEFINED BEHAVIOR!
                              // Might print garbage, crash, or "work" by luck

    std::cout << sv << '\n';   // ⚠️ UNDEFINED BEHAVIOR!
                              // Might print garbage, crash, or "work" by luck

    // SOLUTION: REVALIDATE the view
    // Create new view pointing to the UPDATED string
    sv = s;  // ✓ sv now views s's NEW buffer
    
    // Now it's safe again!
    std::cout << sv << '\n';  // ✓ Prints: "Hello, universe!"
    
    // IMPORTANT LESSONS:
    // 1. Modifying std::string may invalidate views to it
    // 2. Always revalidate views after modifying the underlying string
    // 3. string_view is low-level - YOU manage validity!
    // 4. When in doubt, recreate the view after string modifications
    // 
    // OPERATIONS THAT MAY INVALIDATE VIEWS:
    // - Assignment (s = "new value")
    // - Resize (s.resize())
    // - Append if capacity exceeded (s += "text")
    // - clear(), erase(), etc.
    // 
    // SAFE PATTERN:
    // After ANY modification to std::string, revalidate all views to it!

    return 0;
}