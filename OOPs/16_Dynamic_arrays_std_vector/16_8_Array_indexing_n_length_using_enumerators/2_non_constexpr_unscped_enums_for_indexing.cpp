/**
 * Using non-constexpr enum variables for indexing with explicit underlying type
 * 
 * Key Concepts:
 * 1. Enumerators are constexpr, but enum variables are NOT constexpr
 * 2. Specifying underlying type (: unsigned int) prevents narrowing conversion warnings
 * 3. Enum variables can hold any enumerator value at runtime
 * 4. Difference between enumerators and enum variables
 * 
 * Important Distinction:
 * - Enumerators (Students::Rama): implicitly constexpr, safe conversion
 * - Enum variables (Students::Names name): NOT constexpr, may need underlying type
 */

#include <iostream>
#include <vector>

namespace Students
{
    // Enum with explicitly specified underlying type
    // ': unsigned int' ensures the enum values are stored as unsigned int
    // This prevents narrowing conversion warnings when used with unsigned indices
    enum Names : unsigned int
    {
        Rama,           // 0
        Laxmana,        // 1
        Bharata,        // 2
        Shathragna,     // 3
        max_student     // 4
    };
}

int main()
{
    std::vector<int> testScores{94, 85, 76, 70};
    
    // Create an enum variable (NOT constexpr)
    // While enumerators themselves are constexpr, variables of enum type are not
    // Using the enumerator directly: testScores[Students::Shathragna] - always safe
    // Using an enum variable: testScores[name] - needs underlying type specification
    // 
    // Without ': unsigned int' in enum definition, this could cause narrowing
    // conversion warnings when compiled with strict flags
    Students::Names name{Students::Shathragna};     
    
    // Using non-constexpr enum variable for indexing
    // Safe because we specified underlying type as unsigned int
    testScores[name] = 75;

    // Print all scores
    for(auto i{0Z}; i<std::ssize(testScores); i++)
        std::cout << testScores.data()[i] << " ";  // Output: 94 85 76 75

    std::cout << "\n";

    return 0;
}