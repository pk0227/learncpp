//The default argument can be declared in either the forward declaration or the function definition, but not both.
#include <iostream>

void print(int x, int y); // forward declaration, no default argument

int main()
{
    // print(6);   // COMPILE ERROR: default argument for y hasn't been defined yet at this point in the translation unit
    print(6, 4);   // OK: passing both arguments explicitly works
    return 0;
}

void print(int x, int y=4)
{
    std::cout << "x: " << x << '\n';
    std::cout << "y: " << y << '\n';
}
