#include <iostream>

void print(int a = 10, int b = 20, int c = 30)
{
    std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;
}

int main()
{
    print(1, 2, 3); // All explicit args
    print(1, 2);    // rightmost arg defaulted
    print(1);       // two rightmost args defaulted
    print();        // all args defaulted
    return 0;
}