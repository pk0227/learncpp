#include <iostream>

void foo(unsigned int)
{
    std::cout << "unsigned int foo" << std::endl;
}

void foo(float)
{
    std::cout << "float foo" << std::endl;
}

int main()
{
    foo(0);   // 0 is of type int. int can be converted to both unsigned int and float. So, ambiguity occurs.
    foo(3.14159); // 3.14159 is of type double. double can be converted to both unsigned int and float. So, ambiguity occurs.
    return 0;
}