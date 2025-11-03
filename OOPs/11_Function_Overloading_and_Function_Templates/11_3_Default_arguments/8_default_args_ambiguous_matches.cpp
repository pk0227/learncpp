#include <iostream>

void print(int i = 100)
{
    std::cout << i << '\n';
}

void print(double d = 123.45)
{
    std::cout << d << '\n';
}

int main()
{
    print();    // ambiguous function call
    return 0;
}