#include <iostream>

// 1. All return statements in an auto function must deduce to the SAME type:
/*
auto someFcn(bool b)
{
    if (b)
        return 5;   // return type int
    else
        return 6.7; // return type double -> COMPILE ERROR: inconsistent deduction for auto return type
}
*/

auto someFcn(bool b)
{
    if (b)
        return 5.0; // OK: double
    else
        return 6.7; // OK: double
}

// 2. Functions with auto return type must be defined before use (cannot use forward declaration alone):
// auto foo(); // COMPILE ERROR: use of 'auto foo()' before deduction of 'auto'

auto foo()
{
    return 5;
}

int main()
{
    std::cout << someFcn(true) << '\n';
    std::cout << foo() << '\n';

    return 0;
}