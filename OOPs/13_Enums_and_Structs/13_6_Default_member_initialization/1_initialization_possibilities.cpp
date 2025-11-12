#include <iostream>

struct Something
{
    double x;       // no default initialization value (bad)
    double y {};    // value-initialized by default
    double z { 2 }; // explicit default value
};

std::ostream& operator<<(std::ostream& out, const struct Something& ref)
{
    out << "x : " << ref.x << "\ny : " << ref.y << "\nz : " << ref.z;
    return out;
}

int main()
{
    Something s1;             // No initializer list: s1.x is uninitialized, s1.y and s1.z use defaults
    Something s2 { 5, 6, 7 }; // Explicit initializers: s2.x, s2.y, and s2.z use explicit values (no default values are used)
    Something s3 {};          // Missing initializers: s3.x is value initialized, s3.y and s3.z use defaults

    std::cout << s1 << "\n-----------------\n";
    std::cout << s2 << "\n-----------------\n";
    std::cout << s3 << "\n-----------------\n";

    return 0;
}