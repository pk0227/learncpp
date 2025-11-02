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
    print(1L);      // long can be converted to both int and double. So, ambiguity occurs.
    return 0;
}