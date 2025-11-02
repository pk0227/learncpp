#include <iostream>

int main()
{
    char c { 'a' };
    int x{10};
    int y{4};

    std::cout << (double)x / y << '\n'; // C-style cast of x to double
    std::cout << double(x) / y << '\n'; //  // function-style cast of x to double

    std::cout << static_cast<int>(c) << '\n'; // prints 97 rather than a
    std::cout << static_cast<double>(x) / y << '\n'; // prints 2.5

    // a C-style string literal can't be converted to an int, so the following is an invalid conversion
    //int x{static_cast<int>("Hello")}; // invalid: will produce compilation error

    int i{48};

    // explicit conversion from int to char, so that a char is assigned to variable ch, otherwise compiler
    // would warn about narrowing conversion from int to char.
    char ch{static_cast<char>(i)};

    char ch1{i}; // warning: narrowing conversion from int to char

    return 0;
}