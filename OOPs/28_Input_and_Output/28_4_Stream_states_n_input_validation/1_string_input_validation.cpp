// Demonstrates string input validation using std::ranges and character classification
// Validates that user input contains only alphabetic characters and spaces
// Uses modern C++20 ranges for concise and expressive validation logic

#include <iostream>
#include <string>
#include <string_view>     // For efficient string viewing without copying
#include <cctype>          // For std::isalpha(), std::isspace()
#include <ranges>          // For std::ranges::all_of()
#include <algorithm>

// Validates that a string contains only alphabetic characters and spaces
// Returns true if all characters pass validation, false otherwise
bool isValidString(std::string_view sv)  // string_view avoids copying, efficient for read-only access
{
    // Modern C++20 approach: std::ranges::all_of with direct container
    //return std::ranges::all_of(sv.begin(), sv.end(), [](char ch)  // Explicit iterator version
    return std::ranges::all_of(sv, [](char ch)  // Simpler: ranges work directly with containers
    {
        // Check if character is alphabetic (a-z, A-Z) OR whitespace (space, tab, etc.)
        return std::isalpha(ch) || std::isspace(ch);
    });
}

int main()
{
    std::string str{};

    // Input validation loop: keep prompting until valid input received
    do
    {
        std::cout << "Enter string : ";
        std::getline(std::cin, str);  // Read entire line including spaces
        
        // Loop continues if validation fails
        // Invalid characters: digits (0-9), punctuation (!@#$), special chars
        // Valid characters: letters (a-z, A-Z) and whitespace
    }while(!isValidString(str));  // Repeat until isValidString returns true

    std::cout << "Valid string : " << str << "\n";

    return 0;
}