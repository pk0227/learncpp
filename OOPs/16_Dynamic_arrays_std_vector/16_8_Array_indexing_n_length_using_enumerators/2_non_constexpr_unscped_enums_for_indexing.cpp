#include <iostream>
#include <vector>

namespace Students
{
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
    
    Students::Names name{Students::Shathragna};     // unscoped enumerations will implicitly convert to a std::size_t
                                                    // Because enumerators are implicitly constexpr, But, enum variable is not.
                                                    // conversion of an enumerator to an unsigned integral type may be considered
                                                    // a narrowing conversion, thus avoiding signed/unsigned indexing problems.
                                                    // SOLUTION : Explicitly specifies the underlying type is unsigned int
    testScores[name] = 75;        // non-constexpr

    for(auto i{0Z}; i<std::ssize(testScores); i++)
        std::cout << testScores.data()[i] << " ";

    std::cout << "\n";

    return 0;
}