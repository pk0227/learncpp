#include <iostream>

void printDouble(double d)
{
    std::cout << d << '\n';
}

int main()
{
    printDouble(4.6f);  // numeric promotion of float to double
    printDouble(5.2);   // no conversion necessary

    return 0;
}