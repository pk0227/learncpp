#include <iostream>

void foo(unsigned int)
{

}

int main()
{
    int i{5};
    unsigned int ui{i};     // compile error: list initialization disallows narrowing conversion if compiled with -Werror flag
                            // and i is non-constexpr. If i is constexpr int i;, it will compile.

    foo(i);                 // possible warning: copy initialization allows narrowing conversion

    std::cout << "Unsigned in : " << ui << "\n";

    return 0;
}