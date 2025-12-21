/*
 * Demonstration: std::string_view Conversion Limitations
 * 
 * IMPORTANT RULE:
 * std::string_view does NOT implicitly convert to std::string
 * 
 * WHY?
 * - Implicit conversion would create a copy (defeating the purpose!)
 * - string_view is meant to avoid copies
 * - Must explicitly request conversion to make intent clear
 * 
 * CONVERSION OPTIONS:
 * 1. Explicit construction: std::string s{ sv };
 * 2. Explicit cast: static_cast<std::string>(sv)
 * 
 * DESIGN RATIONALE:
 * - Prevents accidental expensive copies
 * - Forces programmer to acknowledge the cost
 * - Keeps performance characteristics visible in code
 */

#include <iostream>
#include <string>
#include <string_view>

// Function expects std::string (by value = makes a copy)
void printString(std::string str)
{
	std::cout << str << '\n';
}

int main()
{
	std::string_view sv{ "Hello, world!" };

	// COMPILE ERROR: No implicit conversion from string_view to string
	// This would create a copy, which is expensive
	// C++ forces you to be explicit about this cost
	// printString(sv);   // ✗ Won't compile: no implicit conversion

	// SOLUTION 1: Explicitly construct std::string from string_view
	// This makes the copy operation visible and intentional
	std::string s{ sv };  // ✓ Create std::string from string_view (copy made)
	printString(s);       // ✓ Now we can call the function

	// SOLUTION 2: Explicit cast at call site
	// More concise, clearly shows conversion happening
	printString(static_cast<std::string>(sv)); // ✓ Explicit cast = copy allowed
	
	// KEY TAKEAWAY:
	// string_view → string requires explicit action
	// This is intentional to prevent accidental expensive copies
	// Always prefer string_view parameters for read-only functions!

	return 0;
}