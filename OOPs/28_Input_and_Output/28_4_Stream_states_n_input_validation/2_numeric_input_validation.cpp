// Demonstrates numeric input validation by checking string characters
// Validates that user input contains only digit characters (0-9)
// Uses std::ranges to check all characters before conversion

#include <iostream>
#include <string>
#include <string_view>     // For efficient string viewing without copying
#include <cctype>          // For std::isdigit()
#include <ranges>          // For std::ranges::all_of()
#include <algorithm>

// Validates that a string contains only numeric digit characters (0-9)
// Returns true if all characters are digits, false otherwise
// Note: This validation does NOT accept negative numbers (no '-' sign)
// Note: This validation does NOT accept decimal numbers (no '.' point)
bool isValidString(std::string_view sv)  // string_view avoids copying
{
    // Check if ALL characters are digits using std::ranges::all_of
    //return std::ranges::all_of(sv.begin(), sv.end(), [](char ch)  // Explicit iterator version
    return std::ranges::all_of(sv, [](char ch)  // Simpler: ranges work directly with containers
    {
        // std::isdigit returns true for '0'-'9', false for everything else
        return std::isdigit(ch);
    });
}

int main()
{
    std::string str{};

    // Input validation loop: keep prompting until valid numeric input received
    do
    {
        std::cout << "Enter Number : ";
        std::getline(std::cin, str);  // Read entire line as string
        
        // Loop continues if validation fails
        // Invalid: "-123" (negative), "12.34" (decimal), "12a34" (letters), "12 34" (spaces)
        // Valid: "0", "123", "999999" (positive integers only)
    }while(!isValidString(str));  // Repeat until all characters are digits

    // At this point, str is guaranteed to contain only digit characters
    // Can safely convert to int using std::stoi(str) or similar if needed
    std::cout << "Valid Number : " << str << "\n";

    return 0;
}