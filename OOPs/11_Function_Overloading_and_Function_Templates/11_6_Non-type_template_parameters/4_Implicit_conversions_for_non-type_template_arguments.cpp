#include <iostream>

template <int N>
void print()
{
    std::cout << "N : " << N << '\n';
}

int main()
{
    print<5>();
    print<'A'>();

    return 0;
}