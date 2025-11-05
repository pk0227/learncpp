#include <iostream>

// This function template can handle many types, so its implementation is generic
template<typename T>
T max(T x, T y)
{
    std::cout << "Called from template function\n"; 
    return (x < y) ? y : x;
}

int max(int x, int y)
{
    std::cout << "Called from normal function\n";
    return (x < y) ? y : x;
}

int main()
{
    std::cout << max<int>(1,2) << '\n'; // specifying we want to call max<int>. It calls max<int>(int, int)
    std::cout << max<>(1,2) << '\n';    // the compiler will only consider max<int> template function overloads when determining which overloaded function to call.
    std::cout << max(1,2) << '\n';      // the compiler will consider both max<int> template function overloads and max non-template function overloads. Since both are equally viable, the non-template function will be preferred.

    return 0;
}