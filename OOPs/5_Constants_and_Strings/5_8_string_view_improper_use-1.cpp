/*
 * Demonstration: DANGEROUS - string_view Dangling Reference (Scope Issue)
 * 
 * THE PROBLEM:
 * - string_view is just a pointer to string data
 * - If the underlying string is destroyed, the view becomes INVALID
 * - Accessing invalid view = Undefined Behavior (crash, garbage, or "works by luck")
 * 
 * RULE:
 * The viewed string must outlive the string_view!
 * 
 * COMMON MISTAKE:
 * Creating string in inner scope, viewing it in outer scope
 */

#include <iostream>
#include <string>
#include <string_view>

int main()
{
    std::string_view sv{};  // Empty string_view (not viewing anything yet)

    { // Nested block begins
        // Create std::string LOCAL to this nested block
        std::string s{ "Hello, world!" };
        
        // sv now views s (stores pointer to s's data)
        sv = s;
        
        // At this point, sv is VALID (s exists)
        
    } // BLOCK ENDS: s is destroyed!
      // s's memory is deallocated
      // sv still holds pointer to that deallocated memory
      // sv is now a DANGLING REFERENCE

    // UNDEFINED BEHAVIOR!
    // sv points to memory that was freed
    // Might print garbage, might crash, might "work" by accident
    std::cout << sv << '\n';  // ☠️ DANGER: Accessing destroyed string!
    
    // LESSON:
    // Never let a string_view outlive the string it views!
    // The underlying string must remain valid for the entire lifetime of the view

    return 0;
}