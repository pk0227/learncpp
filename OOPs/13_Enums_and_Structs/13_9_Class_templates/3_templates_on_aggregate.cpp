#include <iostream>
/*
template <typename T>
struct Pair
{
    T first{};
    T second{};
};
*/
/*
template <typename T>   // Class templates with template type and non-template type members
struct Pair
{
    T first{};          // first will have whatever type T is replaced with
    int second{};       // second will always have type int, regardless of what type T is
};
*/
template <typename T, typename U>   // Class templates with multiple template types
struct Pair
{
    T first{};
    U second{};
};

template <typename T, typename U>
constexpr auto max(Pair<T, U> p)
{    
    return (p.first < p.second ? p.second : p.first);
}

int main()
{
    Pair<int, double> p1{5, 6.2};
    std::cout << "larger value is : " << max<int, double>(p1) << "\n";

    //Pair p2{5.2, 6.2};      // error: narrowing conversion from 'double' to 'int' as we use initializer list{}, static_cast to int or direct initialization () can be used to avoid compilation error.
    Pair p2{8.2, 7};      
    std::cout << "larger value is : " << max(p2) << "\n";
    return 0;
}