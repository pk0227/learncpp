/**
 * Demonstration of range-based for loops (for-each loops)
 * 
 * Key Concepts:
 * 1. Range-based for loops provide cleaner syntax for iterating containers
 * 2. 'auto' keyword for automatic type deduction
 * 3. const reference (const auto&) prevents copying and modification
 * 4. std::views::reverse for iterating in reverse without modifying the container
 * 
 * Advantages over traditional loops:
 * - No manual index management
 * - No possibility of off-by-one errors
 * - More readable and expressive code
 */

#include <iostream>
#include <vector>
#include <string>
#include <ranges>  // For std::views::reverse (C++20)

int main()
{
    // Vector of strings
    std::vector<std::string> vstr{"Rama", "Seetha", "Krishna", "Rukmini"};

    // Range-based for loop (for-each style)
    // Explicit type: for(const std::string& str : vstr)
    // Using auto for type deduction - compiler deduces std::string
    // const auto& avoids copying strings and prevents modification
    for(const auto& str : vstr)
        std::cout << str << " ";  // Output: Rama Seetha Krishna Rukmini
    
    std::cout << "\n";

    // Reverse iteration using std::views::reverse (C++20)
    // Creates a reverse view without modifying the original container
    // More efficient than creating a reversed copy
    for(const auto& str : std::views::reverse(vstr))
        std::cout << str << " ";  // Output: Rukmini Krishna Seetha Rama
    
    std::cout << "\n";

    return 0;
}