#include <iostream>

class Base
{
public:
    void whichClass() const { std::cout << "Base class\n"; }
};

class Derived : public Base
{
};

int main()
{
    Derived d{};
    d.whichClass();
    return 0;
}