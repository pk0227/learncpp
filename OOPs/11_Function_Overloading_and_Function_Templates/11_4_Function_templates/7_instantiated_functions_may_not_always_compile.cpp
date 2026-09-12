#include <iostream>
#include <string>

template <typename T>
T addOne(T x)
{
    return x+1;
}

int main()
{
    std::string str{"Hello World"};
    // std::cout << addOne(str) << '\n'; // COMPILE ERROR: instantiated addOne<std::string> fails because std::string + int is invalid

    // Valid instantiations where operator+ with int is supported:
    std::cout << addOne(5) << '\n';     // OK: T = int -> 6
    std::cout << addOne(2.5) << '\n';   // OK: T = double -> 3.5
    return 0;
}