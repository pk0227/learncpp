#include <iostream>

void func(int)
{
    std::cout << "int func" << std::endl;
}

void func(double)
{
    std::cout << "double func" << std::endl;
}

int main()
{
    func('a');      // promoted to match foo(int)
    func(true);     // promoted to match foo(int)
    func(4.5f);     // promoted to match foo(double)

    return 0;
}