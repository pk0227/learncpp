#include <iostream>

auto max(auto x, auto y)
{
    return (x < y) ? y : x;
}

int main()
{
    std::cout << max(1, 2.3) << '\n';   
    std::cout << max(2.3, 1) << '\n';

    std::cout << max('A', 1) << '\n';
    std::cout << max('A', 100) << '\n';

    return 0;
}