#include <iostream>

auto someFcn(bool b)
{
    if (b)
        return 5; // return type int
    else
        return 6.7; // return type double
}


auto foo();


int main()
{
    //someFcn(true);  // error: conflicting return types (int vs. double)
    std::cout << foo() << '\n'; // the compiler has only seen a forward declaration at this point
                                // use of ‘auto foo()’ before deduction of ‘auto’ error.
    return 0;
}

auto foo()
{
    return 5;
}