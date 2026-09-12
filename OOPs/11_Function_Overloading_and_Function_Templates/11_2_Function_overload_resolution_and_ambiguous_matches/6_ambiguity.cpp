#include <iostream>

void print(int)
{
    std::cout << "int print function" << std::endl;
}

void print(double)
{
    std::cout << "double print function" << std::endl;
}

int main()
{
    // print(1L);      // COMPILE ERROR: long can be converted to both int and double via numeric conversion, causing ambiguous match

    // Resolution: explicitly cast to the intended type
    print(static_cast<int>(1L));
    print(static_cast<double>(1L));
    return 0;
}