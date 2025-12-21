/*

When using CTAD to initialize a constexpr std::array, the compiler deduces the array size from the number of initializers. If too few initializers are provided, the array becomes shorter than intended, and later indexing may cause undefined behavior.

To prevent this, use static_assert to sanity-check the array size at compile time:

Catch missing initializers (e.g., when adding a new enum value but forgetting its entry).

Ensure multiple constexpr std::array objects have the same length.

Key idea:
Use static_assert to enforce expected array sizes when CTAD is involved.

*/


#include <iostream>
#include <array>

namespace StudentName
{
    enum Students
    {
        rama,           // 0
        laxmana,        // 1   
        bharata,        // 2
        shathragna,     // 3
        hanuma,         // 4
        max_students    // 5
    };
}

int main()
{
    // Correct version: 5 students, 5 scores
    //constexpr std::array testScores{84, 52, 75, 67, 87};
    
    // BUG: Only 4 values provided but we have 5 students!
    constexpr std::array testScores{84, 52, 75, 67}; // Oops, missing Hanuma's score
    
    // Without static_assert, this would compile but cause undefined behavior:
    //std::cout << "Hanuma got a score of " << testScores[StudentName::hanuma] << "\n";   // Index 4 out of bounds!
    
    // static_assert prevents compilation if array size doesn't match enum count
    // This line will cause a compile-time error, catching the bug early
    static_assert(std::size(testScores) == StudentName::max_students);
    
    // If we fix the array, this line would be safe
    std::cout << "Hanuma got a score of " << testScores[StudentName::hanuma] << "\n";
    
    return 0;
}