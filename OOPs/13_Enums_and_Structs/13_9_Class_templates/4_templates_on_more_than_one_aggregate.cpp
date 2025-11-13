#include <iostream>

template <typename T, typename U>   // Class templates with multiple template types
struct Pair
{
    T first{};
    U second{};
};

struct Set
{
    int first{};
    int second{};
};

template <typename T>
constexpr auto max(T p)
{    
    return (p.first < p.second ? p.second : p.first);
}

int main()
{
    Pair<int, double> p1{5, 6.2};
    //std::cout << "larger value is : " << max<int, double>(p1) << "\n";  // in these type of cases, max function should not explicitly mention template arguments. if we do so, it will cause compilation error.
    std::cout << "larger value is : " << max(p1) << "\n";

    Set s1{50, 20};
    std::cout << "larger value is : " << max(s1) << "\n";

    return 0;
}