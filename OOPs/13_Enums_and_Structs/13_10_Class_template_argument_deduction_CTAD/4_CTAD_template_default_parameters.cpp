#include <iostream>

template <typename T=int, typename U=int>
struct Point
{
    T first{};
    U second{};
};

template <typename T, typename U>
Point(T, U) -> Point<T, U>;

//template <typename T, typename U>
//void print(Point<T, U> p)
template <typename T>
void print(T p)
{
    std::cout << "first : " << p.first << "\nsecond : " << p.second << "\n";
}

int main()
{
    Point<int, double> p1{10, 20.123};
    print(p1);
    std::cout << "-------------------------------------\n";

    Point p2{1.23, 4};      // // CTAD deduces automatically
    print(p2);
    std::cout << "-------------------------------------\n";

    Point p3;           // uses default Point<int, int>
    print(p3);

    return 0;
}