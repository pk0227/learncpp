#include <iostream>
#include <vector>

namespace Students
{
    enum Names
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
    testScores[Students::Shathragna] = 75;          // unscoped enumerations will implicitly convert to a std::size_t
                                                    // Because enumerators are implicitly constexpr, conversion of an enumerator 
                                                    // to an unsigned integral type is not considered a narrowing conversion, 
                                                    // thus avoiding signed/unsigned indexing problems.

    for(auto i{0Z}; i<std::ssize(testScores); i++)
        std::cout << testScores.data()[i] << " ";

    std::cout << "\n";

    return 0;
}