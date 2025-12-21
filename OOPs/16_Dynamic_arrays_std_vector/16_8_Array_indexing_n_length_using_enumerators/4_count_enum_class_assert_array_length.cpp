/**
 * Using scoped enumerations (enum class) with operator overloading
 * 
 * Key Concepts:
 * 1. enum class (scoped enum) doesn't implicitly convert to integral types
 * 2. Overloading unary + operator for explicit conversion to underlying type
 * 3. std::underlying_type_t to get the underlying integral type
 * 4. Using count enumerator with scoped enums
 * 
 * Advantages of enum class:
 * - Stronger type safety (no implicit conversions)
 * - No name pollution in enclosing scope
 * - More explicit code when conversion is needed
 */

#include <iostream>
#include <vector>
#include <cassert>
#include <type_traits>      // for std::underlying_type_t

// Scoped enumeration (enum class) for stronger type safety
enum class Students         
{
    Rama,           // 0
    Laxmana,        // 1
    Bharata,        // 2
    Shathragna,     // 3
    max_student     // 4 - Count enumerator
};
// enum class enumerators are accessed as Students::Rama (not just Rama)
// They do NOT implicitly convert to integers

/**
 * Overload unary + operator to convert Students to its underlying type
 * 
 * This provides a convenient way to explicitly convert enum class values
 * to integers when needed (e.g., for array indexing)
 * 
 * @param s - Students enum class value
 * @return The underlying integral value (int by default)
 * 
 * Usage: +Students::Rama returns 0
 */
constexpr auto operator+(Students s) noexcept
{
    // std::underlying_type_t<Students> gets the underlying type (int by default)
    return static_cast<std::underlying_type_t<Students>>(s);
}

int main()
{
    // Must use +Students::max_student to convert to integral type
    // Without the + operator, this would be a compilation error
    std::vector<int> testScores(+Students::max_student);
    
    // Using + operator to convert enum class to index
    testScores[+Students::Shathragna] = 76;

    std::vector<int> testScores1{94, 85, 76, 70};

    // Ensure the number of test scores matches the number of students
    // Must use + to convert enum class to integral type for comparison
    assert(std::size(testScores1) == +Students::max_student);

    // Print all scores
    for(auto i{0Z}; i<std::ssize(testScores); i++)
        std::cout << testScores.data()[i] << " ";  // Output: 0 0 0 76

    std::cout << "\n";

    return 0;
}