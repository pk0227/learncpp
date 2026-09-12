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
    // foo(0);       // COMPILE ERROR: 0 is int, which converts via numeric conversion to both unsigned int and float (ambiguous match)
    // foo(3.14159); // COMPILE ERROR: 3.14159 is double, which converts to both unsigned int and float (ambiguous match)

    // Resolution: explicitly cast or use appropriate literal suffix
    foo(0u);                  // calls foo(unsigned int)
    foo(3.14159f);            // calls foo(float)
    foo(static_cast<unsigned int>(0));
    foo(static_cast<float>(3.14159));

    return 0;
}