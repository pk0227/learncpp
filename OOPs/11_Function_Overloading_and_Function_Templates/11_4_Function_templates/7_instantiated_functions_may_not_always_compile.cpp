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
    std::cout << addOne(str) << '\n';
    return 0;
}