#include <iostream>

template <typename T>   // this is the template parameter declaration defining T as a type template parameter
T max(T x, T y)         // this is the function template definition for max<T>
{
    return (x < y) ? y : x;
}

int main()
{
    std::cout << "max : " << max<int>(1,2) << '\n';
    return 0;
}