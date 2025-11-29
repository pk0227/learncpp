#include <iostream>

class Base
{
public:
    void print(int) { std::cout << "Base - print - int\n"; }
    void print(double) { std::cout << "Base - print - double\n"; }
};

class Derived : public Base
{
};

int main()
{
    Derived d{};
    d.print(5);
    
    return 0;
}