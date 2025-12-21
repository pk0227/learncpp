/**
 * Using constexpr unscoped enumerations for array indexing
 * 
 * Key Concepts:
 * 1. Enumerators provide meaningful names for array indices
 * 2. Enumerators are implicitly constexpr, avoiding narrowing conversion issues
 * 3. Unscoped enums implicitly convert to integral types (std::size_t)
 * 4. Using 'max_student' enumerator as a count/size sentinel
 * 
 * Benefits:
 * - Self-documenting code (Names instead of magic numbers)
 * - No narrowing conversion warnings
 * - Compile-time constants for safety
 */

#include <iostream>
#include <vector>

// Namespace to group related enumerators
namespace Students
{
    // Unscoped enumeration for student names/indices
    enum Names
    {
        Rama,           // 0 - First student
        Laxmana,        // 1 - Second student
        Bharata,        // 2 - Third student
        Shathragna,     // 3 - Fourth student
        max_student     // 4 - Count of students (useful for array sizing)
    };
}

int main()
{
    // Vector sized for number of students
    std::vector<int> testScores{94, 85, 76, 70};
    
    // Using enum for indexing - more readable than testScores[3] = 75
    // Unscoped enumerations implicitly convert to std::size_t
    // Because enumerators are implicitly constexpr, the conversion
    // is NOT considered narrowing, avoiding signed/unsigned issues
    testScores[Students::Shathragna] = 75;

    // Loop using signed size (0Z is a signed size_t literal suffix)
    // std::ssize() returns signed size to avoid unsigned comparison issues
    for(auto i{0Z}; i<std::ssize(testScores); i++)
        std::cout << testScores.data()[i] << " ";  // Output: 94 85 76 75

    std::cout << "\n";

    return 0;
}