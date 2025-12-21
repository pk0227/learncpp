/*
 * Demonstration: CAREFUL - Returning string_view from Parameters
 * 
 * SCENARIO:
 * - Function receives string_view parameters
 * - Function returns one of those parameters
 * - Safety depends on what the CALLER passes!
 * 
 * THE RISK:
 * If caller passes temporary, returned view becomes invalid
 * when temporary is destroyed (at end of full expression)
 * 
 * KEY INSIGHT:
 * Function is safe, but CALLER might misuse it!
 */

#include <iostream>
#include <string>
#include <string_view>

// Returns whichever parameter comes first alphabetically
// This function itself is SAFE
// It just returns one of the views it received
std::string_view firstAlphabetical(std::string_view s1, std::string_view s2)
{
    if (s1 < s2)
        return s1;  // Return first parameter
    return s2;      // Return second parameter
}  // Parameters s1, s2 die here
   // BUT: They're just views, not the actual data
   // Whether returned view is valid depends on what caller passed!

int main()
{
    // CASE 1: SAFE - Passing persistent std::string objects
    std::string a { "World" };
    std::string b { "Hello" };
    // Both a and b outlive the function call and the result usage
    std::cout << firstAlphabetical(a, b) << '\n';  // ✓ Prints: "Hello" - Safe!

    using namespace std::string_literals;
    
    // CASE 2: SAFE - Using result immediately (in same expression)
    // "india"s and "Bharat"s create temporary std::string objects
    // Temporaries destroyed at end of this statement
    // BUT: Result is used before they're destroyed (in same std::cout)
    std::cout << firstAlphabetical("india"s, "Bharat"s) << '\n';  // ✓ Safe - immediate use
    

    // CASE 3: DANGEROUS - Storing result from temporaries
    // Step 1: "india"s and "Bharat"s create temporary std::string objects
    // Step 2: firstAlphabetical returns string_view to one of them
    // Step 3: Temporaries destroyed at end of this statement (after semicolon)
    // Step 4: sv is now a dangling reference!
    std::string_view sv{firstAlphabetical("india"s, "Bharat"s)};
    
    // UNDEFINED BEHAVIOR!
    // sv views a temporary that was already destroyed
    std::cout << sv << '\n';  // ☠️ DANGER: Accessing destroyed temporary!
    
    // KEY RULES:
    // 1. Temporaries live until end of full expression (statement)
    // 2. Using result immediately (same expression) = SAFE
    // 3. Storing result for later = UNSAFE if arguments were temporaries
    // 4. Always ensure viewed data outlives the view!

    return 0;
}