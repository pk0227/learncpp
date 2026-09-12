//The default argument can be declared in either the forward declaration or the function definition, but not both.

#include <iostream>

void print(int x, int y=4); // forward declaration

int main()
{
    print(6);
    return 0;
}

// void print(int x, int y=4) // COMPILE ERROR: default argument cannot be redeclared in definition if already declared in forward declaration
void print(int x, int y)
{
    std::cout << "x: " << x << '\n';
    std::cout << "y: " << y << '\n';
}
