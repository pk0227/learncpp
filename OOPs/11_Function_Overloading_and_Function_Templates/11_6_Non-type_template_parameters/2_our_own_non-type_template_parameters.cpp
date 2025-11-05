#include <iostream>

template<int N>
void print()
{
    std::cout << N << '\n';
}

int main()
{
    print<5>();
    return 0;
}