/*
 * Demonstration: std::string_view Assignment and constexpr
 * 
 * KEY CONCEPTS:
 * 1. Assignment changes what the string_view is viewing (rebinding)
 * 2. Assignment does NOT modify the original string
 * 3. constexpr string_view creates compile-time string constants
 * 
 * IMPORTANT:
 * - string_view is just a "pointer + length" pair
 * - Assigning makes it point to different data
 * - Original data remains unchanged
 */

#include <iostream>
#include <string>
#include <string_view>

int main()
{
    // Create a std::string
    std::string name { "Rama" };
    
    // Create string_view that views 'name'
    std::string_view sv { name };
    std::cout << sv << '\n'; // Prints: Rama

    // REBINDING: Assign new string to view
    // This changes what sv is viewing, NOT the original string!
    sv = "Seetha";  // sv now views the literal "Seetha"
                     // 'name' is completely unchanged
    
    std::cout << sv << '\n';    // Prints: Seetha (sv's new view)
    std::cout << name << '\n';  // Prints: Rama (original unchanged!)
    
    // PROOF: string_view assignment is just rebinding, not modification
    // If assignment modified the string, 'name' would change
    // But it didn't! sv just points to different data now

    std::cout << name << '\n';  // Prints: Rama (original unchanged!)
    
    // PROOF: string_view assignment is just rebinding, not modification
    // If assignment modified the string, 'name' would change
    // But it didn't! sv just points to different data now

	// ========== constexpr string_view ==========
	
	// constexpr: Value must be known at compile-time
	// String literals are compile-time constants
	// Result: s becomes a "string symbolic constant"
	constexpr std::string_view s{ "Hello, world!" };
	
	// At compile-time, everywhere 's' is used, compiler substitutes "Hello, world!"
	// Zero runtime cost - the string view is embedded in the compiled code
    std::cout << s << '\n';
    
    // BENEFITS of constexpr string_view:
    // 1. Compile-time verification (catches errors early)
    // 2. Can be used in constant expressions
    // 3. No runtime initialization cost
    // 4. Perfect for string constants that never change

    return 0;
}