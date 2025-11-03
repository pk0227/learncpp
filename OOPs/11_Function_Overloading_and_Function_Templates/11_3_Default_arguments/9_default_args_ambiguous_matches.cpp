#include <iostream>

void print(int i)
{
    std::cout << i << '\n';
}

void print(int i, int j = 123)
{
    std::cout << j << '\n';
}

void print(int i, double d = 123.45)
{
    std::cout << d << '\n';
}

int main()
{
    print(1,2);
    print(1, 2.5);
    //print(1);    // ambiguous function call
    return 0;
}