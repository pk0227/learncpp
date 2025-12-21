/*
 * Demonstration: string_view View Modification Functions
 * 
 * IMPORTANT CLARIFICATION:
 * - These functions modify the VIEW, not the underlying string!
 * - Original string data remains completely unchanged
 * - Only the "window" (pointer + length) changes
 * 
 * VIEW MODIFICATION FUNCTIONS:
 * 1. remove_prefix(n) - Move start pointer forward by n characters
 * 2. remove_suffix(n) - Reduce length by n characters from end
 * 
 * MEMORY MODEL:
 * Original: "Peach"
 *           [P][e][a][c][h]
 *            ^──────────^ view spans all 5 chars
 * 
 * After remove_prefix(1):
 *           [P][e][a][c][h]
 *               ^───────^ view starts at 'e', length 4
 * 
 * After remove_suffix(2):
 *           [P][e][a][c][h]
 *               ^──^ view: 'e' and 'a' only, length 2
 */

#include <iostream>
#include <string_view>

int main()
{
	// Initialize string_view with string literal
	// View: entire "Peach" (5 characters)
	std::string_view str{ "Peach" };
	std::cout << str << '\n';  // Prints: Peach  // Prints: Peach

	// MODIFY VIEW: Remove 1 character from the LEFT
	// Original string unchanged: "Peach" still exists in memory
	// View now: pointer moved forward 1 position
	// Result: Viewing "each" (4 characters)
	str.remove_prefix(1);
	std::cout << str << '\n';  // Prints: each

	// MODIFY VIEW: Remove 2 characters from the RIGHT
	// Original string still unchanged
	// View now: length reduced by 2
	// Result: Viewing "ea" (2 characters)
	str.remove_suffix(2);
	std::cout << str << '\n';  // Prints: ea

	// RESET VIEW: Assignment makes view point to original literal again
	// This doesn't "undo" anything - just creates a new view
	str = "Peach";
	std::cout << str << '\n';  // Prints: Peach
	
	// KEY POINTS:
	// 1. Original "Peach" literal never changed
	// 2. Only the view (pointer + length) was modified
	// 3. These are cheap operations (just pointer arithmetic)
	// 4. Perfect for parsing strings without creating substrings

	return 0;
}