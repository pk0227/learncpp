#include <iostream>

// Trailing return type syntax (introduced in C++11):
// auto funcName(parameter_list) -> ReturnType

// 1. Basic trailing return type:
auto add(int a, int b) -> int
{
    return a + b;
}

// 2. Trailing return type makes complex return types cleaner to read and write:
auto getGreeting() -> const char*
{
    return "Hello, C++!";
}

// 3. Trailing return type allows return type to depend on parameters:
template <typename T, typename U>
auto multiply(T t, U u) -> decltype(t * u)
{
    return t * u;
}

int main()
{
    std::cout << "add(3, 4): " << add(3, 4) << '\n';
    std::cout << "greeting: " << getGreeting() << '\n';
    std::cout << "multiply(2, 3.5): " << multiply(2, 3.5) << '\n';

    return 0;
}
