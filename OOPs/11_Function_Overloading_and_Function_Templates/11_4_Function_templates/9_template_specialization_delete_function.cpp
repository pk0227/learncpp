#include <iostream>
#include <string>

template <typename T>
T addOne(T x)
{
    return x+1;
}

template<>
const char* addOne(const char *) = delete;

int main()
{
    std::cout << addOne(1) << '\n';
    std::cout << addOne(1u) << '\n';
    std::cout << addOne(1L) << '\n';
    std::cout << addOne(1LL) << '\n';
    std::cout << addOne(1.2f) << '\n';
    std::cout << addOne(1.2) << '\n';
    std::cout << addOne(1.2f) << '\n';
    std::cout << addOne("Hello") << '\n';
    return 0;
}