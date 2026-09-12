#include <iostream>

consteval int goo(int c)    // c is not constexpr, and cannot be used in constant expressions
{
    return c;
}

constexpr int foo(int b)    // b is not constexpr, and cannot be used in constant expressions
{
    // constexpr int b2 { b }; // COMPILE ERROR: constexpr variable requires constant expression initializer
    // return goo(b);          // COMPILE ERROR: consteval function call requires constant expression argument

    return b;                  // OK: b can be returned, but cannot initialize a constexpr variable
}

int main()
{
    constexpr int a { 5 };

    std::cout << foo(a); // okay: constant expression a can be used as argument to constexpr function foo()

    return 0;
}