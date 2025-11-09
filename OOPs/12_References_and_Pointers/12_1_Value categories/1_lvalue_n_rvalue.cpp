#include <iostream>

int getValue()
{
    return 6;
}

int main()
{
    int x{3};               // 5 is an rvalue expression
    const double d{1.23};   // 1.23 is an rvalue expression

    int y{x};           // x is modifiable lvalue expression
    const double e{d};  // d is non-modifiable lvalue expression

    int z{getValue()};  // getValue is an rvalue expression (since the result is returned by value)

    int w{x+1};         // x + 1 is an rvalue expression
    int q{static_cast<int>(d)}; // the result of static casting d to an int is an rvalue expression

    std::cout << "x : " << x << std::endl;
    std::cout << "d : " << d << std::endl;
    std::cout << "z : " << z << std::endl;
    std::cout << "w : " << w << std::endl;
    std::cout << "q : " << q << std::endl;

    std::cout << 1 + 2 << '\n'; // 1 and 2 are rvalues, operator+ returns an rvalue

    // Assignment requires the left operand to be a modifiable lvalue expression and the right operand to be an rvalue expression
    x = 5; // valid: x is a modifiable lvalue expression and 5 is an rvalue expression
    //5 = x; // error: 5 is an rvalue expression and x is a modifiable lvalue expression

    int m{ 1 };
    int n{ 2 };

    m = n; // n is not an rvalue, but this is legal

    return 0;
}