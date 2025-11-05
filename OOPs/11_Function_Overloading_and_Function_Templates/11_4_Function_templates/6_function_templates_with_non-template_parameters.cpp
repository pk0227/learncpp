#include <iostream>
// T is a type template parameter
// double is a non-template parameter
// We don't need to provide names for these parameters since they aren't used
template <typename T>
//int someFcn(T, double)
T someFcn(T, double)
{
    return 5;
}

int main()
{
    std::cout << someFcn(1, 3.4) << typeid(someFcn(1, 3.4)).name() <<'\n'; // matches someFcn(int, double)
    std::cout << someFcn(1, 3.4f) << typeid(someFcn(1, 3.4f)).name() << '\n'; // matches someFcn(int, double) -- the float is promoted to a double
    std::cout << someFcn(1.2, 3.4) << typeid(someFcn(1.2, 3.4)).name() << '\n'; // matches someFcn(double, double)
    std::cout << someFcn(1.2f, 3.4) << typeid(someFcn(1.2f, 3.4)).name() << '\n'; // matches someFcn(float, double)
    std::cout << someFcn(1.2f, 3.4f) << typeid(someFcn(1.2f, 3.4f)).name() << '\n'; // matches someFcn(float, double) -- the float is promoted to a double

    return 0;
}