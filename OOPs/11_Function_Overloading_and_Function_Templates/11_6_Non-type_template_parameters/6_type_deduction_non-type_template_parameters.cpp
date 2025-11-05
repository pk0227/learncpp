#include <iostream>

template <auto N>
void print()
{
    std::cout << N << '\n';
}

int main()
{
    print<5>();
    print<'A'>();
    
    return 0;
}