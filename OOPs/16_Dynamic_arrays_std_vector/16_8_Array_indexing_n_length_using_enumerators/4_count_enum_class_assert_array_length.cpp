#include <iostream>
#include <vector>
#include <cassert>
#include <type_traits>      // for std::underlying_type_t

enum class Students         // now an enum class
{
    Rama,           // 0
    Laxmana,        // 1
    Bharata,        // 2
    Shathragna,     // 3
    max_student     // 4        // count enumerator
};

// Overload the unary + operator to convert StudentNames to the underlying type
constexpr auto operator+(Students s) noexcept
{
    return static_cast<std::underlying_type_t<Students>>(s);
}

int main()
{
    std::vector<int> testScores(+Students::max_student);
    
    testScores[+Students::Shathragna] = 76;

    std::vector<int> testScores1{94, 85, 76, 70};

    // Ensure the number of test scores is the same as the number of students
    assert(std::size(testScores1) == +Students::max_student);

    for(auto i{0Z}; i<std::ssize(testScores); i++)
        std::cout << testScores.data()[i] << " ";

    std::cout << "\n";

    return 0;
}