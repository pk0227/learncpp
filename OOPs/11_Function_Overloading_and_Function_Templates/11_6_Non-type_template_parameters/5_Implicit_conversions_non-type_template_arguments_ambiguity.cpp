#include <iostream>

template <int N>
void print()
{
    std::cout << "N : " << N << '\n';
}

template <char C>
void print()
{
    std::cout << "C : " << C << '\n';
}

int main()
{
    print<5>();
    print<'A'>();

    return 0;
}