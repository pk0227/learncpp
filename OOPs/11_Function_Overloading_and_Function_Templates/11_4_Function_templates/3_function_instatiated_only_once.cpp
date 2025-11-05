#include <iostream>

template <typename T>   // function template for max(T, T)
T max(T x, T y)
{
    return (x < y) ? y : x;
}

int main()
{
    std::cout << "max int : " << max<int>(1,2) << '\n';         // instantiates and calls function max<int>(int, int)
    std::cout << "max int : " << max<int>(3,2) << '\n';         // calls already instantiated function max<int>(int, int)
    std::cout << "max int : " << max<double>(1, 2) << '\n';     // instantiates and calls function max<double>(double, double)
                                                                //actual type is double but args passed are int 
    return 0;
}