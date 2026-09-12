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
    // print();    // COMPILE ERROR: ambiguous function call because both print(int) and print(double) have default arguments

    // Disambiguated calls:
    print(10);     // calls print(int)
    print(3.14);   // calls print(double)
    return 0;
}