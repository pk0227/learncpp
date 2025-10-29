#include <iostream>

class UD
{
    public:
        operator int(){ return 0; }
};

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
    UD x;
    func(x);    // x is converted to type int using the user-defined conversion from UD to int

    return 0;
}