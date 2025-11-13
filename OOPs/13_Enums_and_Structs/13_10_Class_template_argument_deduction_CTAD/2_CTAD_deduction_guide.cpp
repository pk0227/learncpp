#include <iostream>

template <typename T, typename U>
struct Point
{
    T first{};
    U second{};
};

template <typename T, typename U>
Point(T, U) -> Point<T, U>;

int main()
{
    Point<int, double> p1{10, 20.123};
    Point p2{1.23, 45};                     // error: class template argument deduction failed in C++17: It can be fixed with deduction guide

    std::cout << "Thanks!\n";
    return 0;
}