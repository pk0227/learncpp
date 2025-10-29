#include <iostream>

int add(int a, int b)
{
    return a+b;
}

int add(int a, int b, int c)
{
    return a+b+c;
}

int main(int argc, char** argv)
{
    std::cout << "adding 2 params : " << add(10, 20) << std::endl;
    std::cout << "adding 3 params : " << add(10, 20, 30) << std::endl;
    return 0;
}