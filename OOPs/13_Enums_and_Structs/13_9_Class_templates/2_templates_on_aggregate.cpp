#include <iostream>
/*
template <typename T>
struct Pair
{
    T first{};
    T second{};
};
*/

template <typename T>   // Class templates with template type and non-template type members
struct Pair
{
    T first{};          // first will have whatever type T is replaced with
    int second{};       // second will always have type int, regardless of what type T is
};


template <typename T>
constexpr T max(Pair<T> p)
{    
    return (p.first < p.second ? p.second : p.first);
}

int main()
{
    Pair<int> p1{5, 6};
    std::cout << "larger value is : " << max<int>(p1) << "\n";

    //Pair p2{5.2, 6.2};      // error: narrowing conversion from 'double' to 'int' as we use initializer list{}, static_cast to int or direct initialization () can be used to avoid compilation error.
    Pair p2{5.2, 7};      
    std::cout << "larger value is : " << max(p2) << "\n";
    return 0;
}