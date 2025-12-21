/*
 * Demonstration: string_view May NOT Be Null-Terminated
 * 
 * CRITICAL ISSUE:
 * - C-style strings are null-terminated (end with \0)
 * - std::string_view is NOT guaranteed to be null-terminated
 * - Especially after using remove_prefix/remove_suffix or viewing substring
 * 
 * THE DANGER:
 * - std::cout << sv might read past the view's length looking for \0
 * - Can print garbage characters after the intended string
 * - Undefined behavior!
 * 
 * SAFE OUTPUT METHOD:
 * Use .write() with explicit length instead of <<
 */

#include <iostream>

// Function demonstrates safe vs unsafe string_view printing
void print(std::string_view sv)
{
    // These functions work correctly (return the view's length)
	std::cout << "size : " << std::ssize(sv) << "\n";   // Signed size
	std::cout << "size : " << sv.size() << "\n";        // Unsigned size
	std::cout << "length : " << sv.length() << "\n";    // Same as size()

	// UNSAFE: Assumes null-termination (may not exist!)
	// If sv views substring, might print garbage after "Hello"
	//std::cout << sv << "\n";  // ⚠️ DANGER: Undefined behavior!
	
	// SAFE: Explicitly specify how many characters to print
	// .data() returns pointer to first character
	// .size() returns number of characters to print
	std::cout.write(sv.data(), sv.size()) << std::endl;
	// This ONLY prints the characters in the view, guaranteed!
}

int main()
{
    // Create substring view: std::string constructor with start position and length
    // Original string: "Hello World"
    // Substring: Starting at index 0, take 5 characters = "Hello"
    // 
    // PROBLEM: This creates a string_view of a TEMPORARY std::string!
    // After this statement, the temporary is destroyed
    // The view becomes invalid (dangling reference)
    // 
    // This code has TWO bugs:
    // 1. Viewing a temporary (undefined behavior)
    // 2. Even if string were permanent, substring might not be null-terminated
    print(std::string{"Hello World", 0, 5});
    
    // CORRECT APPROACH (if you needed substring):
    // std::string str{"Hello World"};
    // std::string_view sv{str.data(), 5};  // View first 5 chars
    // print(sv);
    
    return 0;
}