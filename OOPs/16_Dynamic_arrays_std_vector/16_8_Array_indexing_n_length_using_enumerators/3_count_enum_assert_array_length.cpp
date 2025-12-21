/**
 * Using count enumerator for array sizing and compile-time assertions
 * 
 * Key Concepts:
 * 1. Count enumerator pattern: max_student represents the total count
 * 2. Using count enumerator for vector sizing
 * 3. Static assertions to validate array/vector size at compile time
 * 4. Ensuring array size matches the number of enumerators
 * 
 * Benefits:
 * - Automatic size calculation as enumerators are added/removed
 * - Compile-time safety with assertions
 * - Self-documenting code
 */

#include <iostream>
#include <vector>
#include <cassert>

namespace Students
{
    enum Names : unsigned int
    {
        Rama,           // 0
        Laxmana,        // 1
        Bharata,        // 2
        Shathragna,     // 3
        max_student     // 4 - Count enumerator (total number of students)
    };
    // If we add more students, max_student automatically adjusts
}

int main()
{
    // Using count enumerator to size the vector
    // This ensures the vector has exactly enough elements for all students
    // Direct initialization with size (creates vector with 4 zero-initialized elements)
    std::vector<int> testScores(Students::max_student);
    
    // Now we can safely use any enumerator as an index
    testScores[Students::Shathragna] = 76;

    // Separate vector initialized with specific values
    std::vector<int> testScores1{94, 85, 76, 70};

    // Runtime assertion to ensure the vector size matches the enum count
    // This catches errors if we forget to update the initializer list
    // Will abort the program if the assertion fails
    assert(std::size(testScores1) == Students::max_student);

    // Print all scores from the sized vector
    for(auto i{0Z}; i<std::ssize(testScores); i++)
        std::cout << testScores.data()[i] << " ";  // Output: 0 0 0 76

    std::cout << "\n";

    return 0;
}