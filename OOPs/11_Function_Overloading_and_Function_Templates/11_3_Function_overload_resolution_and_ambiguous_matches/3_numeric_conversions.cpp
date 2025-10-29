#include <iostream>
#include <string>

void func(double)
{
    std::cout << "double func" << std::endl;
}

void func(std::string)
{
    std::cout << "string func" << std::endl;
}

int main()
{
    func('a');  // 'a' converted to match foo(double)
    return 0;
}